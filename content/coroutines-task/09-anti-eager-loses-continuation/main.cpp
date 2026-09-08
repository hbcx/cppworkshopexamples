#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

// Counts task completions that finished with nobody waiting for them. In a real
// async task that is a lost wakeup: the result has nowhere to go and any caller
// expecting to be resumed never will be.
long g_orphaned = 0;

// A task whose eagerness is a compile-time choice. Everything else -- the
// continuation, the symmetric transfer back to the caller -- is identical.
template <bool Eager>
struct Task {
    struct promise_type {
        int result = 0;
        std::coroutine_handle<> continuation{};

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        auto initial_suspend() noexcept {
            if constexpr (Eager) return std::suspend_never{};    // start at creation
            else                 return std::suspend_always{};   // start when awaited
        }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> cont = h.promise().continuation;
                if (!cont) ++g_orphaned;      // finished before anyone attached
                return cont ? cont : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(int value) { result = value; }
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }

    struct Awaiter {
        std::coroutine_handle<promise_type> callee;
        // If the task already finished (eager), there is nothing to wait for.
        bool await_ready() const noexcept { return callee.done(); }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) const noexcept {
            callee.promise().continuation = awaiting;
            return callee;
        }
        int await_resume() const noexcept { return callee.promise().result; }
    };
    Awaiter operator co_await() const noexcept { return Awaiter{handle}; }
};

// A plain top-level driver. It is driven by hand, so its own completion has no
// continuation -- keeping it a separate type means only the inner tasks above
// touch g_orphaned.
struct RootTask {
    struct promise_type {
        RootTask get_return_object() {
            return RootTask{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
    explicit RootTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    RootTask(RootTask&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    RootTask(const RootTask&) = delete;
    ~RootTask() { if (handle) handle.destroy(); }
};

Task<false> lazyInner() {
    std::cout << "  inner body runs\n";
    co_return 42;
}

Task<true> eagerInner() {
    std::cout << "  inner body runs\n";
    co_return 42;
}

RootTask lazyDriver() {
    std::cout << "  creating task (nothing runs yet)\n";
    Task<false> t = lazyInner();
    std::cout << "  awaiting it now\n";
    int v = co_await t;                     // continuation is set BEFORE inner runs
    std::cout << "  got " << v << "\n";
    co_return;
}

RootTask eagerDriver() {
    std::cout << "  creating task\n";
    Task<true> t = eagerInner();            // inner runs right here, unwitnessed
    std::cout << "  awaiting it now\n";
    int v = co_await t;                     // inner already finished; value read directly
    std::cout << "  got " << v << "\n";
    co_return;
}

int main() {
    g_orphaned = 0;
    std::cout << "LAZY task (correct):\n";
    { RootTask d = lazyDriver(); d.handle.resume(); }
    std::cout << "  orphaned completions: " << g_orphaned << "\n\n";

    g_orphaned = 0;
    std::cout << "EAGER task (anti):\n";
    { RootTask d = eagerDriver(); d.handle.resume(); }
    std::cout << "  orphaned completions: " << g_orphaned
              << "  (finished before anyone waited -- in real async, a lost wakeup)\n";
    return 0;
}
