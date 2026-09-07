#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <utility>

template <typename T>
struct Generator {
    struct promise_type {
        T current{};
        std::exception_ptr error;   // stash a thrown exception here

        Generator get_return_object() {
            return Generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) { current = std::move(value); return {}; }
        void return_void() {}
        // Called with the exception in flight, inside the coroutine: capture it.
        void unhandled_exception() { error = std::current_exception(); }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    Generator(Generator&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Generator(const Generator&) = delete;
    ~Generator() { if (handle) handle.destroy(); }

    bool next() {
        handle.resume();
        // Rethrow into the caller's context if the body threw.
        if (handle.promise().error)
            std::rethrow_exception(handle.promise().error);
        return !handle.done();
    }
    const T& value() const { return handle.promise().current; }
};

Generator<int> risky() {
    co_yield 1;
    co_yield 2;
    throw std::runtime_error("boom at the 3rd value");
}

int main() {
    auto g = risky();
    try {
        while (g.next())
            std::cout << "got " << g.value() << "\n";
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << "\n";
    }
    return 0;
}
