#include <coroutine>
#include <iostream>

// A minimal coroutine return type. It starts suspended (lazy) and we drive it by
// hand through the handle -- see the generator section for the full walkthrough.
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

// An AWAITER: the object a co_await expression drives through three hooks.
// This one is "already ready", so it never actually suspends the coroutine.
struct ReadyValue {
    int value;

    // 1. Called first. Return true to mean "no need to suspend, carry on".
    bool await_ready() const noexcept {
        std::cout << "  await_ready -> true (no suspend)\n";
        return true;
    }
    // 2. Skipped entirely when await_ready() returned true.
    void await_suspend(std::coroutine_handle<>) const noexcept {
        std::cout << "  await_suspend (not reached here)\n";
    }
    // 3. Called as the coroutine proceeds; its result is the value of co_await.
    int await_resume() const noexcept {
        std::cout << "  await_resume -> " << value << "\n";
        return value;
    }
};

Task demo() {
    std::cout << "coroutine body start\n";
    int x = co_await ReadyValue{42};   // await_ready true -> no suspension
    std::cout << "got " << x << "\n";
}

int main() {
    std::cout << "creating coroutine (starts suspended)\n";
    Task t = demo();
    std::cout << "resuming\n";
    t.resume();
    std::cout << "done? " << std::boolalpha << t.done() << "\n";
    return 0;
}
