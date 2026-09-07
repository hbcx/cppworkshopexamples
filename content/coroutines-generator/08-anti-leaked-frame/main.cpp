#include <coroutine>
#include <iostream>

// A local whose destructor is observable, so we can see whether the frame that
// holds it was ever cleaned up.
long g_destroyed = 0;
struct Tracked {
    ~Tracked() { ++g_destroyed; }
};

// A DELIBERATELY unsafe return type: it exposes the raw handle and does NOT
// destroy the frame in its destructor. That is the whole point of the example.
struct Coro {
    struct promise_type {
        Coro get_return_object() {
            return Coro{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;   // no destructor -> no cleanup
};

Coro makeCoro() {
    Tracked local;                     // lives in the frame while suspended
    co_await std::suspend_always{};     // suspend with `local` still alive
    (void)local;
}

int main() {
    // ANTI-PATTERN: enter the body, then drop the handle without destroy().
    // The frame (and `local` inside it) is never released.
    g_destroyed = 0;
    {
        Coro c = makeCoro();
        c.handle.resume();             // run into the body: `local` is constructed
        // c goes out of scope here -- but Coro has no destructor, so the frame LEAKS
    }
    std::cout << "leaked path:  Tracked destroyed = " << g_destroyed << "\n";  // 0

    // FIX: destroy() the frame (what an RAII wrapper does in its destructor).
    g_destroyed = 0;
    {
        Coro c = makeCoro();
        c.handle.resume();
        c.handle.destroy();            // releases the frame -> `local` destructor runs
    }
    std::cout << "destroyed path: Tracked destroyed = " << g_destroyed << "\n"; // 1
    return 0;
}
