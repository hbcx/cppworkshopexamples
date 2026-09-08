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

    T get() {
        if (handle.promise().error)
            std::rethrow_exception(handle.promise().error);
        return std::move(handle.promise().result);
    }
};

Task<int> doubler(int x) {
    co_return x * 2;
}

// Three dependent async steps, written as straight-line code. Each co_await
// waits for the previous result before the next call is even made.
Task<int> pipeline() {
    int a = co_await doubler(5);     // 10
    std::cout << "  after step 1: " << a << "\n";
    int b = co_await doubler(a);     // 20
    std::cout << "  after step 2: " << b << "\n";
    int c = co_await doubler(b);     // 40
    std::cout << "  after step 3: " << c << "\n";
    co_return c;
}

int main() {
    Task<int> t = pipeline();
    t.handle.resume();
    std::cout << "pipeline result = " << t.get() << "\n";
    return 0;
}
