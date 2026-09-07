#include <coroutine>
#include <iostream>

// A local whose destructor is observable, so we can see whether the frame that
// holds it was ever cleaned up.
long g_destroyed = 0;
struct Tracked {
    ~Tracked() { ++g_destroyed; }
};

// A fire-and-forget coroutine: it starts running at once (suspend_never) and its
// frame frees itself when it finishes (suspend_never). Nobody owns a handle to
// it, so the ONLY way it is ever cleaned up is by running to the end.
struct DetachedTask {
    struct promise_type {
        DetachedTask get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// The "scheduler": the one handle that is ready to be resumed.
std::coroutine_handle<> g_ready{};

// ANTI-PATTERN: await_suspend suspends the coroutine but never registers the
// handle anywhere, so nothing can ever resume it. The coroutine is stranded
// mid-body forever, and because it is detached, its frame (and Tracked) leaks.
struct BrokenWait {
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<>) const noexcept {
        // BUG: the handle is dropped on the floor. g_ready stays empty.
    }
    void await_resume() const noexcept {}
};

// FIX: hand the handle to the scheduler so something CAN resume it.
struct GoodWait {
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        g_ready = h;
    }
    void await_resume() const noexcept {}
};

DetachedTask worker(bool broken) {
    Tracked local;                 // lives in the frame while suspended
    if (broken)
        co_await BrokenWait{};
    else
        co_await GoodWait{};
    (void)local;
}

int main() {
    // ANTI: the handle is never registered -> nothing to resume -> stranded, leaks.
    g_destroyed = 0;
    g_ready = {};
    worker(true);
    if (g_ready) { g_ready.resume(); }   // never taken: g_ready is empty
    std::cout << "broken: Tracked destroyed = " << g_destroyed
              << "  (leaked: suspended forever, handle was never registered)\n";

    // FIX: the handle reaches the scheduler -> we resume it -> it runs to the end.
    g_destroyed = 0;
    g_ready = {};
    worker(false);
    if (g_ready) {
        std::coroutine_handle<> h = g_ready;
        g_ready = {};
        h.resume();
    }
    std::cout << "fixed:  Tracked destroyed = " << g_destroyed
              << "  (resumed via the scheduler, ran to completion, frame freed)\n";
    return 0;
}
