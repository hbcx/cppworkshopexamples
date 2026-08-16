#include <future>
#include <iostream>
#include <system_error>

// Anti-pattern: calling get() on the same std::future more than once.
//
// std::future::get() MOVES the result out and leaves the future without a
// shared state (valid() becomes false). A second get() then throws
// std::future_error with future_errc::no_state. get() is a one-shot transfer,
// not a re-readable accessor -- so if you need the value in several places,
// store it, or use a std::shared_future.
long compute() { return 21; }

int main() {
    std::future<long> f = std::async(std::launch::async, compute);

    long first = f.get();                // fine: transfers the value out
    std::cout << "first get  = " << first << '\n';               // 21

    // The trap: get() again on the now-empty future throws (this is defined
    // behaviour -- an exception, not UB -- so we run it and catch it).
    try {
        long second = f.get();
        std::cout << "second get = " << second << '\n';          // never reached
    } catch (const std::future_error& e) {
        bool no_state = (e.code() == std::future_errc::no_state);
        std::cout << "second get threw, no_state = "
                  << std::boolalpha << no_state << '\n';          // true
    }

    // The fix if you truly need many reads: shared_future, whose get() returns a
    // const reference and can be called repeatedly.
    std::shared_future<long> sf =
        std::async(std::launch::async, compute).share();
    std::cout << "shared_future get twice = " << sf.get() << ", "
              << sf.get() << '\n';                                // 21, 21
    return 0;
}
