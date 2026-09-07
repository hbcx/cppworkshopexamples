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

// (1) bool await_suspend: return false to NOT suspend after all (resume this
// coroutine right away), true to suspend. Useful when await_ready cannot decide
// cheaply but await_suspend discovers the result is already there.
struct MaybeSuspend {
    bool should_suspend;
    bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<>) const noexcept { return should_suspend; }
    void await_resume() const noexcept {}
};

// (2) coroutine_handle await_suspend: return another coroutine's handle and the
// runtime resumes THAT one next, without nesting calls on the stack. This is
// "symmetric transfer" -- the safe way to hand control from one coroutine to
// another (see the anti-pattern that resumes inside await_suspend instead).
struct TransferTo {
    std::coroutine_handle<> next;
    bool await_ready() const noexcept { return false; }
    std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept {
        return next;
    }
    void await_resume() const noexcept {}
};

Task inner() {
    std::cout << "    [inner] running via symmetric transfer\n";
    co_return;
}

Task outer(std::coroutine_handle<> innerHandle) {
    std::cout << "  await_suspend -> false: ";
    co_await MaybeSuspend{false};           // does not suspend
    std::cout << "kept going without suspending\n";

    std::cout << "  await_suspend -> true: about to suspend\n";
    co_await MaybeSuspend{true};            // suspends here
    std::cout << "  resumed after the true-suspend\n";

    std::cout << "  await_suspend -> handle: transferring to inner\n";
    co_await TransferTo{innerHandle};       // control jumps to inner
    std::cout << "  back in outer, inner already finished\n";
}

int main() {
    Task in = inner();               // suspended at its start
    Task out = outer(in.handle);     // suspended at its start

    std::cout << "resume outer:\n";
    out.resume();                    // runs through the false-suspend, parks at the true-suspend

    std::cout << "outer parked, resume again:\n";
    out.resume();                    // resumes, then symmetric-transfers into inner

    std::cout << "inner done? " << std::boolalpha << in.done() << "\n";
    std::cout << "outer done? " << out.done() << "\n";   // still parked after the transfer

    out.resume();                    // resume outer past the transfer point
    std::cout << "outer done now? " << out.done() << "\n";
    return 0;
}
