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

// This awaiter DOES suspend: await_ready returns false. await_suspend then runs
// while the coroutine is parked, and control returns to whoever resumed it.
// The coroutine stays suspended until someone resumes its handle again.
struct SuspendOnce {
    bool await_ready() const noexcept { return false; }        // false -> suspend
    void await_suspend(std::coroutine_handle<>) const noexcept {
        std::cout << "  await_suspend: parked, control goes back to the resumer\n";
    }
    void await_resume() const noexcept {
        std::cout << "  await_resume: back in the coroutine body\n";
    }
};

Task demo() {
    std::cout << "  before co_await\n";
    co_await SuspendOnce{};      // splits the body in two at this point
    std::cout << "  after co_await\n";
}

int main() {
    Task t = demo();
    std::cout << "resume #1:\n";
    t.resume();                  // runs up to the co_await, then suspends
    std::cout << "coroutine is parked, done? " << std::boolalpha << t.done() << "\n";
    std::cout << "resume #2:\n";
    t.resume();                  // continues past the co_await to the end
    std::cout << "done? " << t.done() << "\n";
    return 0;
}
