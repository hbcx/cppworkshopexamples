#include <coroutine>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <utility>

// A tiny single-threaded event loop with a VIRTUAL clock. Instead of really
// waiting, a coroutine parks itself at a future time; run() jumps the clock from
// one scheduled wake-up to the next. This makes async timing fully deterministic,
// which is what lets the combinator examples show real interleaving reliably.
struct Loop {
    std::multimap<long, std::coroutine_handle<>> timers;   // wake time -> handle
    long now = 0;

    void sleep_until(long t, std::coroutine_handle<> h) { timers.emplace(t, h); }
    void run() {
        while (!timers.empty()) {
            auto it = timers.begin();
            now = it->first;                 // jump the clock to the next wake-up
            std::coroutine_handle<> h = it->second;
            timers.erase(it);
            h.resume();
        }
    }
};
Loop g_loop;

// Awaiting Sleep suspends the coroutine and schedules it to wake `ms` later.
struct Sleep {
    long ms;
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        g_loop.sleep_until(g_loop.now + ms, h);
    }
    void await_resume() const noexcept {}
};

// The lazy task from the previous section, adapted to fire a callback when it
// finishes -- the combinators use that callback to count completions.
template <typename T>
struct Task {
    struct promise_type {
        T result{};
        std::exception_ptr error{};
        std::function<void()> on_complete;

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            void await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                if (h.promise().on_complete) h.promise().on_complete();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(T value) { result = std::move(value); }
        void unhandled_exception() { error = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }

    void start(std::function<void()> cb) {
        handle.promise().on_complete = std::move(cb);
        handle.resume();                 // lazy: kick it off; runs until it sleeps or ends
    }
    T result() const { return handle.promise().result; }
};

Task<int> nap(int id, long ms) {
    std::cout << "  task " << id << " sleeps for " << ms << "\n";
    co_await Sleep{ms};
    std::cout << "  task " << id << " woke at t=" << g_loop.now << "\n";
    co_return id;
}

int main() {
    Task<int> t = nap(1, 25);
    t.start([]{});                        // start the task; it parks on Sleep
    std::cout << "before run, clock t=" << g_loop.now << "\n";
    g_loop.run();                         // drive the loop until nothing is scheduled
    std::cout << "after run, clock t=" << g_loop.now << ", result=" << t.result() << "\n";
    return 0;
}
