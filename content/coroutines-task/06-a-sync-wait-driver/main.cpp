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

    struct Awaiter {
        std::coroutine_handle<promise_type> callee;
        bool await_ready() const noexcept { return false; }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) const noexcept {
            callee.promise().continuation = awaiting;
            return callee;
        }
        T await_resume() const {
            if (callee.promise().error)
                std::rethrow_exception(callee.promise().error);
            return std::move(callee.promise().result);
        }
    };
    Awaiter operator co_await() const noexcept { return Awaiter{handle}; }

    bool done() const { return handle.done(); }
    T get() {
        if (handle.promise().error)
            std::rethrow_exception(handle.promise().error);
        return std::move(handle.promise().result);
    }
};

// The bridge from ordinary code into the coroutine world: start the task, drive
// it to completion, and return its value. main() and tests are not coroutines,
// so they cannot co_await -- sync_wait is what they call instead. (A real one
// would spin an event loop until the task completes; here every step finishes
// synchronously, so a single resume runs it to the end.)
template <typename T>
T sync_wait(Task<T> task) {
    task.handle.resume();
    return task.get();
}

Task<int> add(int a, int b) {
    co_return a + b;
}

Task<int> program() {
    int x = co_await add(2, 3);      // 5
    int y = co_await add(x, 10);     // 15
    co_return y;
}

int main() {
    int result = sync_wait(program());   // cross from sync code into the task
    std::cout << "sync_wait result = " << result << "\n";
    return 0;
}
