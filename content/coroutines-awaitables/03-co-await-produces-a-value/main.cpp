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

// The awaiter carries a value, suspends, and hands the value back from
// await_resume. Because the awaiter lives inside the coroutine frame across the
// suspension, the value survives it -- co_await evaluates to that value.
struct DelayedValue {
    int value;
    bool await_ready() const noexcept { return false; }         // always suspend
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const noexcept { return value; }         // result of co_await
};

Task demo() {
    std::cout << "asking for two values...\n";
    int a = co_await DelayedValue{10};   // suspends, resumes, a = 10
    int b = co_await DelayedValue{32};   // suspends, resumes, b = 32
    std::cout << "got " << a << " and " << b << ", sum " << (a + b) << "\n";
}

int main() {
    Task t = demo();
    // Each co_await suspends once; drive until the coroutine finishes.
    while (!t.done())
        t.resume();
    return 0;
}
