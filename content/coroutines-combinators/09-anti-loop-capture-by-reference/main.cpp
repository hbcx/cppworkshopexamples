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
};

// Start every task, run the loop to completion, and collect the results in order.
std::vector<int> gather(std::vector<Task<int>> tasks) {
    for (std::size_t i = 0; i < tasks.size(); ++i)
        tasks[i].start([]{});
    g_loop.run();
    std::vector<int> results(tasks.size());
    for (std::size_t i = 0; i < tasks.size(); ++i)
        results[i] = tasks[i].result();
    return results;
}

// A coroutine parameter is captured into the frame. Taken BY REFERENCE, the frame
// stores only the reference, so the task reads its value LATER, when it actually
// runs -- not the value at spawn time.
Task<int> byRef(const int& id, long ms) {
    co_await Sleep{ms};
    co_return id;
}

// BY VALUE, each frame owns its own copy taken at spawn time.
Task<int> byValue(int id, long ms) {
    co_await Sleep{ms};
    co_return id;
}

int main() {
    // ANTI: spawn tasks in a loop, all taking a reference to the SAME variable.
    // By the time they run, that variable holds the last value, so every task
    // reports it. (Binding the reference to the loop variable itself would be
    // worse: it dangles once the loop ends -- undefined behaviour.)
    int shared = 0;
    std::vector<Task<int>> refTasks;
    for (int i = 0; i < 3; ++i) {
        shared = i;
        refTasks.push_back(byRef(shared, 10));   // every task references `shared`
    }
    // `shared` is now 2; all three tasks will read 2.

    // FIX: pass the id by value so each task copies its own at spawn time.
    std::vector<Task<int>> valTasks;
    for (int i = 0; i < 3; ++i)
        valTasks.push_back(byValue(i, 10));

    std::vector<int> refResults = gather(std::move(refTasks));
    g_loop.now = 0;
    std::vector<int> valResults = gather(std::move(valTasks));

    std::cout << "by reference (anti):";
    for (int v : refResults) std::cout << " " << v;
    std::cout << "   <- all read the final value of shared\n";

    std::cout << "by value (fix):     ";
    for (int v : valResults) std::cout << " " << v;
    std::cout << "   <- each task kept its own id\n";
    return 0;
}
