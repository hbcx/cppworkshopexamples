#include <future>
#include <iostream>
#include <thread>

// Anti-pattern: destroying a std::promise without setting a value.
//
// A future expects the producer to deliver exactly one thing: a value or an
// exception. If the promise is destroyed having set NEITHER -- an early return,
// a thrown exception on the producer path, a forgotten set_value -- the shared
// state is marked "broken", and the waiting future.get() throws
// std::future_error with future_errc::broken_promise. Every code path that owns
// a promise must fulfil it.
void producer_buggy(std::promise<int> p, bool take_shortcut) {
    if (take_shortcut) {
        // Early out that forgets to set the promise. When p is destroyed here,
        // the future is broken. (Defined behaviour -- an exception on get(), not
        // UB -- so we run it and catch it below.)
        return;
    }
    p.set_value(7);
}

int main() {
    // --- the trap: producer returns without setting the value ---
    std::promise<int> p;
    std::future<int> f = p.get_future();
    std::thread(producer_buggy, std::move(p), true).join();
    try {
        f.get();
    } catch (const std::future_error& e) {
        bool broken = (e.code() == std::future_errc::broken_promise);
        std::cout << "unset promise: broken_promise = "
                  << std::boolalpha << broken << '\n';           // true
    }

    // --- the fix: fulfil the promise on every path ---
    std::promise<int> p2;
    std::future<int> f2 = p2.get_future();
    std::thread(producer_buggy, std::move(p2), false).join();
    std::cout << "fulfilled promise: value = " << f2.get() << '\n';   // 7
    return 0;
}
