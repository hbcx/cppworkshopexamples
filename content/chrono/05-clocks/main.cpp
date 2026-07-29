#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // is_steady tells you whether now() can ever go backwards. We print the
    // guaranteed properties, never an absolute clock value (which is not portable).
    std::cout << "steady_clock::is_steady = " << std::boolalpha
              << steady_clock::is_steady << '\n';        // true, by definition
    std::cout << "system_clock::is_steady = "
              << system_clock::is_steady << '\n';        // false in practice

    // steady_clock is the right tool for an interval: two reads, and the second
    // is never smaller than the first.
    auto a = steady_clock::now();
    auto b = steady_clock::now();
    std::cout << "steady now() never decreases? " << (b >= a ? "yes" : "no") << '\n';

    // Only system_clock converts to a calendar/time_t (shown in a later example);
    // steady_clock's epoch is unspecified, so its absolute value is meaningless.
    std::cout << "system_clock converts to time_t; steady_clock does not\n";
    return 0;
}
