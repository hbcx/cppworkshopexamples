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

Task<int> make() {
    co_return 21;
}

// ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
//
//   Task<int> t = make();
//   int a = co_await t;        // drives t to its final_suspend (done)
//   int b = co_await t;        // await_suspend returns t.handle and the runtime
//                              // resumes an already-finished coroutine -> UB
//
// A task owns ONE frame and ONE result; awaiting it runs that frame to the end.
// Awaiting the same task object again resumes a coroutine sitting at
// final_suspend, which is undefined. It is a one-shot handle, not a value you
// can read repeatedly.

// FIX (a): await once and reuse the value.
Task<int> reuseTheValue() {
    int v = co_await make();       // await ONCE
    co_return v + v;               // reuse v; do not co_await the same task again
}

// FIX (b): if you really need the work done twice, make a fresh task each time.
Task<int> freshTaskEachTime() {
    int a = co_await make();       // one task
    int b = co_await make();       // a different task
    co_return a + b;
}

int main() {
    Task<int> t1 = reuseTheValue();
    t1.handle.resume();
    std::cout << "reuse the value:      " << t1.get() << "\n";

    Task<int> t2 = freshTaskEachTime();
    t2.handle.resume();
    std::cout << "fresh task each time: " << t2.get() << "\n";
    return 0;
}
