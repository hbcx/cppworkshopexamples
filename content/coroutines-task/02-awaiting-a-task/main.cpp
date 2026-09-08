#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

template <typename T>
struct Task {
    struct promise_type {
        T result{};
        std::exception_ptr error{};
        std::coroutine_handle<> continuation{};

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            // When the task finishes, resume whoever was awaiting it -- a
            // symmetric transfer straight back to the caller, no nesting.
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> cont = h.promise().continuation;
                return cont ? cont : std::noop_coroutine();
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

    // co_await on a Task: record the caller as the task's continuation and start
    // the task; await_resume then hands back the task's result.
    struct Awaiter {
        std::coroutine_handle<promise_type> callee;
        bool await_ready() const noexcept { return false; }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) const noexcept {
            callee.promise().continuation = awaiting;   // remember who to resume
            return callee;                              // run the task now
        }
        T await_resume() const {
            if (callee.promise().error)
                std::rethrow_exception(callee.promise().error);
            return std::move(callee.promise().result);
        }
    };
    Awaiter operator co_await() const noexcept { return Awaiter{handle}; }

    T get() {
        if (handle.promise().error)
            std::rethrow_exception(handle.promise().error);
        return std::move(handle.promise().result);
    }
};

Task<int> inner() {
    co_return 42;
}

Task<int> outer() {
    std::cout << "  outer: awaiting inner\n";
    int v = co_await inner();                 // suspends outer, runs inner, returns here
    std::cout << "  outer: inner returned " << v << "\n";
    co_return v + 1;
}

int main() {
    Task<int> t = outer();
    t.handle.resume();       // one resume drives the whole chain via symmetric transfer
    std::cout << "final result = " << t.get() << "\n";
    return 0;
}
