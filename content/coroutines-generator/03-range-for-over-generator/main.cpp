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

    // A single-pass input iterator over the coroutine.
    struct iterator {
        std::coroutine_handle<promise_type> h = nullptr;
        iterator& operator++() {
            h.resume();
            if (h.done()) h = nullptr;   // reaching the end becomes the end() iterator
            return *this;
        }
        const T& operator*() const { return h.promise().current; }
        bool operator!=(const iterator& o) const { return h != o.h; }
    };

    iterator begin() {
        handle.resume();                 // advance from initial_suspend to the first value
        return iterator{ handle.done() ? std::coroutine_handle<promise_type>{nullptr} : handle };
    }
    iterator end() { return iterator{ nullptr }; }
};

Generator<int> range(int first, int last) {
    for (int i = first; i < last; ++i)
        co_yield i;
}

int main() {
    std::cout << "range-for:";
    for (int x : range(1, 6))
        std::cout << " " << x;
    std::cout << "\n";

    long sum = 0;
    for (int x : range(1, 101))
        sum += x;
    std::cout << "sum 1..100 = " << sum << "\n";
    return 0;
}
