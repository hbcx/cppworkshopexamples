#include <coroutine>
#include <iostream>
#include <utility>

template <typename T>
struct Generator {
    struct promise_type {
        T current{};
        Generator get_return_object() {
            return Generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) { current = std::move(value); return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    Generator(Generator&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Generator(const Generator&) = delete;
    ~Generator() { if (handle) handle.destroy(); }
    bool next() { handle.resume(); return !handle.done(); }
    const T& value() const { return handle.promise().current; }
    bool done() const { return handle.done(); }
};

Generator<int> range(int first, int last) {
    for (int i = first; i < last; ++i)
        co_yield i;
}

int main() {
    // ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
    //
    //   auto g = range(1, 3);            // yields 1, 2, then finishes
    //   for (int i = 0; i < 5; ++i) {    // asks for 5 values from a 2-value source
    //       g.next();                    // the 3rd next() leaves it done()...
    //       std::cout << g.value();      // ...and the 4th resume() inside next()
    //   }                                // resumes a COMPLETED coroutine -> UB
    //
    // Once done() is true, resume() is undefined, and value() reads a dead frame.

    // FIX: make the pull the loop condition, so it stops at the real end even
    // though we were willing to take up to 5.
    auto g = range(1, 3);
    int taken = 0;
    for (int i = 0; i < 5 && g.next(); ++i) {
        std::cout << "got " << g.value() << "\n";
        ++taken;
    }
    std::cout << "asked for up to 5, safely took " << taken << "\n";
    std::cout << "done? " << std::boolalpha << g.done() << "\n";
    return 0;
}
