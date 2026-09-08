#include <coroutine>
#include <exception>
#include <iostream>
#include <map>
#include <utility>

// --- the virtual-time event loop from the combinators section -----------------
struct Loop {
    std::multimap<long, std::coroutine_handle<>> timers;
    long now = 0;
    void sleep_until(long t, std::coroutine_handle<> h) { timers.emplace(t, h); }
    void run() {
        while (!timers.empty()) {
            auto it = timers.begin();
            now = it->first;
            std::coroutine_handle<> h = it->second;
            timers.erase(it);
            h.resume();
        }
    }
};
Loop g_loop;

struct Sleep {
    long ms;
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        g_loop.sleep_until(g_loop.now + ms, h);
    }
    void await_resume() const noexcept {}
};

// --- a minimal consumer task --------------------------------------------------
struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }
    void start() { handle.resume(); }
};

// --- the async generator ------------------------------------------------------
// A stream that both PRODUCES with co_yield and WAITS with co_await. Advancing it
// is itself asynchronous, so the consumer must co_await each step. Control ping-
// pongs by symmetric transfer: next() resumes the producer, co_yield resumes the
// consumer, and while the producer awaits, the whole thing parks on the loop.
template <typename T>
struct async_generator {
    struct promise_type {
        T current{};
        std::exception_ptr error{};
        std::coroutine_handle<> continuation{};      // the consumer awaiting a value

        async_generator get_return_object() {
            return async_generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct YieldAwaiter {
            std::coroutine_handle<> consumer;
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type>) const noexcept {
                return consumer ? consumer : std::noop_coroutine();   // hand the value back
            }
            void await_resume() const noexcept {}
        };
        YieldAwaiter yield_value(T value) {
            current = std::move(value);
            return YieldAwaiter{continuation};
        }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> c = h.promise().continuation;
                return c ? c : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_void() {}
        void unhandled_exception() { error = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit async_generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    async_generator(async_generator&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    async_generator(const async_generator&) = delete;
    ~async_generator() { if (handle) handle.destroy(); }

    struct NextAwaiter {
        std::coroutine_handle<promise_type> h;
        bool await_ready() const noexcept { return false; }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> consumer) const noexcept {
            h.promise().continuation = consumer;
            return h;                                 // resume the producer
        }
        bool await_resume() const {
            if (h.promise().error) std::rethrow_exception(h.promise().error);
            return !h.done();                         // false once the stream ends
        }
    };
    NextAwaiter next() { return NextAwaiter{handle}; }
    T& value() { return handle.promise().current; }
};

async_generator<int> producer() {
    for (int i = 1; i <= 3; ++i) {
        co_await Sleep{10};       // wait for async work, then...
        co_yield i;               // ...produce a value
    }
}

Task consume(async_generator<int>& gen) {
    std::cout << "consuming:";
    while (co_await gen.next())    // advancing is asynchronous -> co_await it
        std::cout << " " << gen.value() << "@t" << g_loop.now;
    std::cout << "\n";
    co_return;
}

int main() {
    async_generator<int> gen = producer();   // owned here -> outlives the consumer
    Task c = consume(gen);
    c.start();                                // runs until the producer first awaits
    g_loop.run();                             // drive the stream to the end
    return 0;
}
