#include <coroutine>
#include <iostream>

// --- Task A: a type that is awaitable via operator co_await --------------------
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

// Event is NOT an awaiter (it has none of the three hooks). It is "awaitable"
// because it provides operator co_await, which returns the actual awaiter.
struct Event {
    int id;
    struct Awaiter {
        int id;
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<>) const noexcept {}
        int await_resume() const noexcept { return id; }
    };
    Awaiter operator co_await() const noexcept { return Awaiter{id}; }
};

Task demoOperator() {
    std::cout << "  awaiting an Event (via operator co_await)\n";
    int id = co_await Event{5};
    std::cout << "  Event delivered id " << id << "\n";
}

// --- Task B: await_transform rewrites what you may co_await --------------------
// A promise with await_transform intercepts every co_await in that coroutine, so
// the coroutine can co_await things that are not awaitable on their own (or the
// promise can forbid some of them). Here a plain int is turned into a ready value.
struct XformTask {
    struct promise_type {
        XformTask get_return_object() {
            return XformTask{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }

        auto await_transform(int n) const noexcept {
            struct Ready {
                int n;
                bool await_ready() const noexcept { return true; }
                void await_suspend(std::coroutine_handle<>) const noexcept {}
                int await_resume() const noexcept { return n; }
            };
            return Ready{n * 10};    // co_await <int> yields n * 10
        }
    };
    std::coroutine_handle<promise_type> handle;
    explicit XformTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    XformTask(XformTask&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    XformTask(const XformTask&) = delete;
    ~XformTask() { if (handle) handle.destroy(); }
    void resume() { handle.resume(); }
    bool done() const { return handle.done(); }
};

XformTask demoTransform() {
    std::cout << "  co_await 7 (rewritten by await_transform)\n";
    int v = co_await 7;
    std::cout << "  got " << v << " (7 * 10)\n";
}

int main() {
    std::cout << "operator co_await:\n";
    {
        Task a = demoOperator();
        while (!a.done())
            a.resume();
    }
    std::cout << "await_transform:\n";
    {
        XformTask b = demoTransform();
        b.resume();
    }
    return 0;
}
