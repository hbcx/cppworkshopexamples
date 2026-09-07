#include <coroutine>
#include <iostream>

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
    void resume() { handle.resume(); }
    bool done() const { return handle.done(); }
};

struct Config {
    int timeout;
};

// ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
//
//   struct WaitRef {
//       const Config& cfg;                       // reference into a temporary
//       bool await_ready() const noexcept { return false; }
//       void await_suspend(std::coroutine_handle<>) const noexcept {}
//       int await_resume() const noexcept { return cfg.timeout; }  // cfg may be gone
//   };
//   int t = co_await WaitRef{ Config{500} };     // the temporary Config dies at the ;
//   // the coroutine suspends; by the time it resumes, cfg dangles -> UB
//
// The awaiter object is stored in the coroutine frame across the suspension, but
// a reference member still points at a temporary that is destroyed when the full
// expression ends -- long before await_resume runs.

// FIX: store the awaited data BY VALUE in the awaiter. The copy lives in the
// frame for as long as the awaiter does, so it is still valid on resume.
struct WaitValue {
    Config cfg;                    // owned copy in the frame
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const noexcept { return cfg.timeout; }
};

Task demo() {
    int t = co_await WaitValue{ Config{500} };   // Config copied into the awaiter
    std::cout << "timeout read after suspension: " << t << "\n";
}

int main() {
    Task task = demo();
    while (!task.done())
        task.resume();
    return 0;
}
