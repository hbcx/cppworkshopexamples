#include <coroutine>
#include <iostream>

// The return type of a coroutine must name a promise_type. The compiler rewrites
// the body into a state machine and uses these hooks to drive it.
struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }  // start suspended
        std::suspend_always final_suspend() noexcept { return {}; }    // stay alive at the end
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(const Task&) = delete;                 // one owner of the frame
    Task& operator=(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }   // WE own the frame; destroy it

    bool done() const { return handle.done(); }
    void resume() { handle.resume(); }
};

// The presence of co_await / co_return makes this a coroutine, not a function.
Task counter() {
    std::cout << "  [coroutine] start\n";
    co_await std::suspend_always{};   // suspend here, hand control back to the caller
    std::cout << "  [coroutine] resumed once\n";
    co_await std::suspend_always{};
    std::cout << "  [coroutine] resumed twice, finishing\n";
    co_return;
}

int main() {
    std::cout << "calling counter() -- body has NOT run yet\n";
    Task t = counter();               // initial_suspend suspends at the top

    std::cout << "resume 1\n"; t.resume();   // runs to the first co_await
    std::cout << "resume 2\n"; t.resume();   // runs to the second co_await
    std::cout << "resume 3\n"; t.resume();   // runs to co_return; now finished

    std::cout << "done? " << std::boolalpha << t.done() << "\n";
    return 0;                          // ~Task destroys the frame
}
