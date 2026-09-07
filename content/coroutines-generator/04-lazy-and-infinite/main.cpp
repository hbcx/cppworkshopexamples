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
};

// No co_return, no end condition: an infinite sequence. Safe because it only
// advances when pulled.
Generator<long> fibonacci() {
    long a = 0, b = 1;
    for (;;) {
        co_yield a;
        long next = a + b;
        a = b;
        b = next;
    }
}

// Count how many values the body actually computes, to prove laziness.
long g_produced = 0;
Generator<long> naturals() {
    for (long n = 0; ; ++n) {
        ++g_produced;
        co_yield n;
    }
}

int main() {
    auto fib = fibonacci();
    std::cout << "first 10 fibonacci:";
    for (int i = 0; i < 10 && fib.next(); ++i)   // consumer decides to stop at 10
        std::cout << " " << fib.value();
    std::cout << "\n";

    g_produced = 0;
    auto nat = naturals();
    long sum = 0;
    for (int i = 0; i < 5 && nat.next(); ++i)
        sum += nat.value();
    std::cout << "sum of first 5 naturals = " << sum << "\n";       // 0+1+2+3+4
    std::cout << "values actually computed = " << g_produced << "\n"; // 5, not infinite
    return 0;
}
