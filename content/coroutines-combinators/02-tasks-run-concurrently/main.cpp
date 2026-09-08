#include <coroutine>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
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
        handle.resume();
    }
    T result() const { return handle.promise().result; }
};

// Track how many tasks are "in flight" (started but not yet finished) at once.
long g_in_flight = 0;
long g_max_in_flight = 0;

Task<int> worker(int id, long ms) {
    ++g_in_flight;
    if (g_in_flight > g_max_in_flight) g_max_in_flight = g_in_flight;
    std::cout << "  worker " << id << " starts (waits " << ms
              << "), in flight now = " << g_in_flight << "\n";
    co_await Sleep{ms};
    std::cout << "  worker " << id << " done at t=" << g_loop.now << "\n";
    --g_in_flight;
    co_return id;
}

int main() {
    Task<int> a = worker(1, 20);
    Task<int> b = worker(2, 10);
    // Start BOTH before either finishes: both park on Sleep and are in flight at
    // the same time on one thread. The loop then wakes them in time order.
    a.start([]{});
    b.start([]{});
    g_loop.run();
    std::cout << "max in flight at once = " << g_max_in_flight << "\n";
    return 0;
}
