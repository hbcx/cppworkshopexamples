#include <coroutine>
#include <iostream>

// initial_suspend returns suspend_never -> the coroutine runs straight away when
// it is created; final_suspend returns suspend_never -> the frame destroys itself
// at the end, so this return type owns no handle at all (fire and forget).
struct EagerTask {
    struct promise_type {
        EagerTask get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }  // start now
        std::suspend_never final_suspend() noexcept { return {}; }    // self-destroy at end
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// initial_suspend returns suspend_always -> the coroutine waits for the first
// resume; final_suspend returns suspend_always -> the frame stays alive after the
// body ends, so the owner must destroy() it. That is why this type holds a handle.
struct LazyTask {
    struct promise_type {
        LazyTask get_return_object() {
            return LazyTask{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }  // wait for resume
        std::suspend_always final_suspend() noexcept { return {}; }    // keep the frame
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
    explicit LazyTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    LazyTask(LazyTask&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    LazyTask(const LazyTask&) = delete;
    ~LazyTask() { if (handle) handle.destroy(); }
    void resume() { handle.resume(); }
    bool done() const { return handle.done(); }
};

EagerTask eager() {
    std::cout << "  [eager] runs at creation, no resume needed\n";
    co_return;
}

LazyTask lazy() {
    std::cout << "  [lazy] runs only when resumed\n";
    co_return;
}

int main() {
    std::cout << "creating eager task:\n";
    eager();                       // body runs here, then the frame frees itself

    std::cout << "creating lazy task:\n";
    LazyTask lz = lazy();          // nothing printed yet -- parked at the start
    std::cout << "resuming lazy task:\n";
    lz.resume();                   // now the body runs
    std::cout << "lazy done, frame still owned: " << std::boolalpha << lz.done() << "\n";
    return 0;
}
