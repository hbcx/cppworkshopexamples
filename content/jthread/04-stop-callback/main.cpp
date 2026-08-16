#include <atomic>
#include <iostream>
#include <stop_token>

// A std::stop_callback runs a function the moment a stop is requested, instead of
// you polling stop_requested() in a loop. It is how you REACT to cancellation
// from outside a worker's own loop: wake a blocked wait, close a socket, roll a
// transaction back. You register it on a stop_token; it fires once, on whichever
// thread calls request_stop().
int main() {
    std::cout << std::boolalpha;

    std::atomic<bool> cleanup_ran{false};
    std::stop_source src;

    // Register a reaction to cancellation. It has not fired yet.
    std::stop_callback cb(src.get_token(), [&cleanup_ran] {
        cleanup_ran.store(true);
    });
    std::cout << "before request: cleanup ran = " << cleanup_ran.load() << '\n'; // false

    src.request_stop();                        // fires cb right here, now
    std::cout << "after request:  cleanup ran = " << cleanup_ran.load() << '\n'; // true

    // Registering a callback when the stop was ALREADY requested runs it
    // immediately, on the registering thread -- so a late listener never misses it.
    std::atomic<bool> late_ran{false};
    std::stop_callback late(src.get_token(), [&late_ran] { late_ran.store(true); });
    std::cout << "late callback ran at once   = " << late_ran.load() << '\n';    // true
    return 0;
}
