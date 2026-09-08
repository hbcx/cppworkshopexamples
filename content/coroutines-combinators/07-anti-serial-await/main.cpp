#include <coroutine>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

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
    int result() const { return handle.promise().result; }

    // Awaiting a task directly starts it and waits for just that one to finish.
    struct Awaiter {
        Task* task;
        std::coroutine_handle<> awaiting{};
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> h) {
            awaiting = h;
            task->start([this] { g_loop.sleep_until(g_loop.now, awaiting); });
        }
        int await_resume() const { return task->result(); }
    };
    Awaiter operator co_await() { return Awaiter{this}; }
};

struct WhenAll {
    std::vector<Task<int>> tasks;
    std::vector<int> results;
    int remaining = 0;
    std::coroutine_handle<> joiner{};
    explicit WhenAll(std::vector<Task<int>> ts) : tasks(std::move(ts)) {
        results.resize(tasks.size());
        remaining = static_cast<int>(tasks.size());
    }
    bool await_ready() const noexcept { return tasks.empty(); }
    void await_suspend(std::coroutine_handle<> h) {
        joiner = h;
        for (std::size_t i = 0; i < tasks.size(); ++i)
            tasks[i].start([this, i] {
                results[i] = tasks[i].result();
                if (--remaining == 0) g_loop.sleep_until(g_loop.now, joiner);
            });
    }
    std::vector<int> await_resume() { return std::move(results); }
};

Task<int> fetch(int id, long ms) {
    co_await Sleep{ms};
    co_return id * 10;
}

// ANTI-PATTERN: three INDEPENDENT fetches awaited one after another. Each await
// runs a task to completion before the next even starts, so the latencies add up
// (20 + 20 + 20 = 60) even though nothing forced them to be sequential.
Task<int> serial() {
    Task<int> a = fetch(1, 20);
    Task<int> b = fetch(2, 20);
    Task<int> c = fetch(3, 20);
    int ra = co_await a;
    int rb = co_await b;
    int rc = co_await c;
    std::cout << "serial (anti):  results " << ra << " " << rb << " " << rc
              << ", finished at t=" << g_loop.now << "\n";
    co_return 0;
}

// FIX: start them together with when_all. They run interleaved, so the total is
// the slowest one (20), not the sum.
Task<int> parallel() {
    std::vector<Task<int>> tasks;
    tasks.push_back(fetch(1, 20));
    tasks.push_back(fetch(2, 20));
    tasks.push_back(fetch(3, 20));
    std::vector<int> r = co_await WhenAll{std::move(tasks)};
    std::cout << "when_all (fix): results";
    for (int v : r) std::cout << " " << v;
    std::cout << ", finished at t=" << g_loop.now << "\n";
    co_return 0;
}

int main() {
    {
        Task<int> p = serial();
        p.start([]{});
        g_loop.run();
    }
    g_loop.now = 0;   // reset the virtual clock for the second run
    {
        Task<int> p = parallel();
        p.start([]{});
        g_loop.run();
    }
    return 0;
}
