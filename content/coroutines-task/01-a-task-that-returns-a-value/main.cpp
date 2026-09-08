#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

// A Task<T>: a coroutine that runs some work and produces a value with co_return.
// The promise stores the result (like the generator's promise stored the current
// value), and the frame stays alive at final_suspend so the caller can read it.
template <typename T>
struct Task {
    struct promise_type {
        T result{};
        std::exception_ptr error{};
        std::coroutine_handle<> continuation{};   // used in later examples

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }   // lazy: waits

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> cont = h.promise().continuation;
                return cont ? cont : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(T value) { result = std::move(value); }       // co_return x
        void unhandled_exception() { error = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }

    // Read the result of a finished top-level task (rethrows if it failed).
    T get() {
        if (handle.promise().error)
            std::rethrow_exception(handle.promise().error);
        return std::move(handle.promise().result);
    }
};

Task<int> compute() {
    co_return 6 * 7;
}

int main() {
    Task<int> t = compute();     // lazy: the body has NOT run yet
    std::cout << "task created, body not run yet\n";
    t.handle.resume();           // kick the lazy task off; it runs to co_return
    std::cout << "result = " << t.get() << "\n";
    return 0;
}
