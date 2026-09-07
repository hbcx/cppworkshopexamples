#include <coroutine>
#include <iostream>
#include <string>
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
};

// Any element type -- here strings.
Generator<std::string> words() {
    co_yield "alpha";
    co_yield "beta";
    co_yield "gamma";
}

Generator<int> range(int first, int last) {
    for (int i = first; i < last; ++i)
        co_yield i;
}

// Compose: consume a source generator (taken BY VALUE, so the frame owns it)
// and yield each element squared. Still lazy -- one value in flight.
Generator<int> squares(Generator<int> src) {
    while (src.next())
        co_yield src.value() * src.value();
}

int main() {
    std::cout << "words:";
    auto w = words();
    while (w.next())
        std::cout << " " << w.value();
    std::cout << "\n";

    std::cout << "squares of 1..5:";
    auto sq = squares(range(1, 6));   // pipeline: range -> squares
    while (sq.next())
        std::cout << " " << sq.value();
    std::cout << "\n";
    return 0;
}
