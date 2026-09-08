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
    std::exception_ptr error() const { return handle.promise().error; }
};

// when_all: start every task, and resume the awaiting coroutine only once the
// LAST of them has finished. Results come back in input order. The awaiter object
// lives in the awaiting coroutine's frame across the suspension, so its members
// (the tasks, the results, the counter) stay valid while the tasks run.
struct WhenAll {
    std::vector<Task<int>> tasks;
    std::vector<int> results;
    std::exception_ptr error{};
    int remaining = 0;
    std::coroutine_handle<> joiner{};

    explicit WhenAll(std::vector<Task<int>> ts) : tasks(std::move(ts)) {
        results.resize(tasks.size());
        remaining = static_cast<int>(tasks.size());
    }

    bool await_ready() const noexcept { return tasks.empty(); }
    void await_suspend(std::coroutine_handle<> h) {
        joiner = h;
        for (std::size_t i = 0; i < tasks.size(); ++i) {
            tasks[i].start([this, i] {
                if (tasks[i].error()) { if (!error) error = tasks[i].error(); }
                else                  results[i] = tasks[i].result();
                if (--remaining == 0)
                    g_loop.sleep_until(g_loop.now, joiner);   // last one: wake the joiner
            });
        }
    }
    std::vector<int> await_resume() {
        if (error) std::rethrow_exception(error);
        return std::move(results);
    }
};

Task<int> fetch(int id, long ms) {
    co_await Sleep{ms};
    co_return id * 10;
}

Task<int> program() {
    std::vector<Task<int>> tasks;
    tasks.push_back(fetch(1, 30));
    tasks.push_back(fetch(2, 10));
    tasks.push_back(fetch(3, 20));
    std::vector<int> r = co_await WhenAll{std::move(tasks)};
    std::cout << "all results (input order):";
    for (int v : r) std::cout << " " << v;
    std::cout << "\nfinished at t=" << g_loop.now << " (the max of 30/10/20, not the sum)\n";
    co_return 0;
}

int main() {
    Task<int> p = program();
    p.start([]{});
    g_loop.run();
    return 0;
}
