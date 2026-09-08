#include <coroutine>
#include <exception>
#include <iostream>

// The Task<void> shape: the same machinery as a value task, but with nothing to
// return. The promise has return_void instead of return_value, there is no
// stored result, and await_resume yields void. It is used to sequence work that
// has effects but no value -- logging steps, writing output, running a stage.
struct Task {
    struct promise_type {
        std::exception_ptr error{};
        std::coroutine_handle<> continuation{};

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> cont = h.promise().continuation;
                return cont ? cont : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_void() {}                      // co_return; with no value
        void unhandled_exception() { error = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }

    struct Awaiter {
        std::coroutine_handle<promise_type> callee;
        bool await_ready() const noexcept { return false; }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) const noexcept {
            callee.promise().continuation = awaiting;
            return callee;
        }
        void await_resume() const {                // nothing to hand back
            if (callee.promise().error)
                std::rethrow_exception(callee.promise().error);
        }
    };
    Awaiter operator co_await() const noexcept { return Awaiter{handle}; }
};

Task stepA() {
    std::cout << "  step A\n";
    co_return;
}

Task stepB() {
    std::cout << "  step B\n";
    co_return;
}

Task run() {
    co_await stepA();          // sequence two void tasks, no value flows
    co_await stepB();
    std::cout << "  both steps done\n";
}

int main() {
    Task t = run();
    t.handle.resume();
    std::cout << "run finished\n";
    return 0;
}
