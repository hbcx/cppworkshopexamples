#include <coroutine>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>

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

template <typename T>
struct async_generator {
    struct promise_type {
        T current{};
        std::exception_ptr error{};
        std::coroutine_handle<> continuation{};
        async_generator get_return_object() {
            return async_generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        struct YieldAwaiter {
            std::coroutine_handle<> consumer;
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type>) const noexcept {
                return consumer ? consumer : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        YieldAwaiter yield_value(T value) { current = std::move(value); return YieldAwaiter{continuation}; }
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
            return h;
        }
        bool await_resume() const {
            if (h.promise().error) std::rethrow_exception(h.promise().error);   // replay it here
            return !h.done();
        }
    };
    NextAwaiter next() { return NextAwaiter{handle}; }
    T& value() { return handle.promise().current; }
};

// The producer yields two values, then fails on the third.
async_generator<int> flaky() {
    co_await Sleep{10};
    co_yield 1;
    co_await Sleep{10};
    co_yield 2;
    co_await Sleep{10};
    throw std::runtime_error("stream failed at the 3rd element");
    co_yield 3;   // unreachable
}

Task consume(async_generator<int>& gen) {
    try {
        while (co_await gen.next())      // the failure surfaces at this co_await
            std::cout << "  got " << gen.value() << "\n";
        std::cout << "  stream ended cleanly (not reached)\n";
    } catch (const std::exception& e) {
        std::cout << "  caught from the stream: " << e.what() << " (at t=" << g_loop.now << ")\n";
    }
    co_return;
}

int main() {
    async_generator<int> gen = flaky();
    Task c = consume(gen);
    c.start();
    g_loop.run();
    return 0;
}
