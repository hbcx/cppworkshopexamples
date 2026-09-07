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
        std::suspend_always initial_suspend() noexcept { return {}; }  // lazy start
        std::suspend_always final_suspend() noexcept { return {}; }
        // co_yield value -> yield_value(value): stash it and suspend.
        std::suspend_always yield_value(T value) {
            current = std::move(value);
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    Generator(Generator&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Generator(const Generator&) = delete;             // move-only: one owner
    ~Generator() { if (handle) handle.destroy(); }

    // Pull the next value; false once the coroutine has run to the end.
    bool next() {
        handle.resume();
        return !handle.done();
    }
    const T& value() const { return handle.promise().current; }
};

// A coroutine: co_yield makes it one. It produces the half-open range [first, last).
Generator<int> range(int first, int last) {
    for (int i = first; i < last; ++i)
        co_yield i;
}

int main() {
    Generator<int> g = range(1, 6);
    std::cout << "pulled:";
    while (g.next())
        std::cout << " " << g.value();
    std::cout << "\n";
    return 0;
}
