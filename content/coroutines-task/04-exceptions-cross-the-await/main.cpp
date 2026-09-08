#include <coroutine>
#include <exception>
#include <iostream>
#include <stdexcept>
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
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> cont = h.promise().continuation;
                return cont ? cont : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(T value) { result = std::move(value); }
        // A throw that escapes the body lands here; stash it to replay later.
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
        // On resume, replay the task's exception into the awaiting coroutine.
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

Task<int> failing() {
    throw std::runtime_error("boom in the task");
    co_return 0;   // never reached, but co_return makes this a coroutine
}

Task<int> caller() {
    std::cout << "  calling a task that throws\n";
    try {
        int v = co_await failing();               // the throw surfaces HERE
        std::cout << "  got " << v << " (not reached)\n";
    } catch (const std::exception& e) {
        std::cout << "  caught across the await: " << e.what() << "\n";
    }
    co_return -1;
}

int main() {
    Task<int> t = caller();
    t.handle.resume();
    std::cout << "caller result = " << t.get() << "\n";
    return 0;
}
