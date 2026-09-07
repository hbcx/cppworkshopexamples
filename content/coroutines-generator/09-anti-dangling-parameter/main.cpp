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

// ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
//
//   Generator<char> chars(const std::string& s) {   // reference parameter
//       for (char c : s) co_yield c;
//   }
//   auto g = chars(std::string("hi"));   // the temporary string dies HERE,
//   g.next();                            // but the body reads `s` now -> UB
//
// The coroutine stores only the reference; the temporary it refers to is gone
// by the time the suspended body runs.

// FIX: take the parameter BY VALUE. It is moved into the coroutine frame and
// lives exactly as long as the coroutine, so a temporary argument is safe.
Generator<char> chars(std::string s) {
    for (char c : s)
        co_yield c;
}

int main() {
    std::cout << "chars of a temporary:";
    auto g = chars(std::string("hello"));   // safe: the string was copied into the frame
    while (g.next())
        std::cout << " " << g.value();
    std::cout << "\n";
    return 0;
}
