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
    // Remove a coroutine's pending wake-up so the loop will not resume it.
    void cancel(std::coroutine_handle<> h) {
        for (auto it = timers.begin(); it != timers.end(); ) {
            if (it->second == h) it = timers.erase(it);
            else ++it;
        }
    }
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

// when_any: resume the awaiting coroutine as soon as the FIRST task finishes, and
// cancel the rest -- remove their pending wake-ups so the loop never resumes them.
// The cancelled tasks are still owned here and destroyed with the awaiter.
struct WhenAny {
    std::vector<Task<int>> tasks;
    bool resolved = false;
    std::size_t winner = 0;
    int value = 0;
    std::coroutine_handle<> joiner{};

    explicit WhenAny(std::vector<Task<int>> ts) : tasks(std::move(ts)) {}

    bool await_ready() const noexcept { return tasks.empty(); }
    void await_suspend(std::coroutine_handle<> h) {
        joiner = h;
        for (std::size_t i = 0; i < tasks.size(); ++i) {
            tasks[i].start([this, i] {
                if (resolved) return;             // a faster task already won
                resolved = true;
                winner = i;
                value = tasks[i].result();
                for (std::size_t j = 0; j < tasks.size(); ++j)
                    if (j != i) g_loop.cancel(tasks[j].handle);   // drop the losers
                g_loop.sleep_until(g_loop.now, joiner);
            });
        }
    }
    std::pair<std::size_t, int> await_resume() { return {winner, value}; }
};

Task<int> fetch(int id, long ms) {
    co_await Sleep{ms};
    co_return id * 10;
}

Task<int> program() {
    std::vector<Task<int>> tasks;
    tasks.push_back(fetch(1, 30));
    tasks.push_back(fetch(2, 10));   // fastest -> wins
    tasks.push_back(fetch(3, 20));
    auto [i, v] = co_await WhenAny{std::move(tasks)};
    std::cout << "first to finish: task index " << i << " -> " << v
              << " at t=" << g_loop.now << "\n";
    std::cout << "the other two were cancelled before they could wake\n";
    co_return 0;
}

int main() {
    Task<int> p = program();
    p.start([]{});
    g_loop.run();
    return 0;
}
