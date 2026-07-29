#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // high_resolution_clock is defined only as "the clock with the shortest tick".
    // The standard does NOT require it to be steady and lets it be a typedef for
    // system_clock or steady_clock -- which one is not portable. If it aliases
    // system_clock, it inherits the ability to jump when the wall clock is
    // corrected, so an interval measured with it can come out negative: the exact
    // bug steady_clock exists to prevent. We do not print the alias (it varies and
    // is not the point) -- the missing guarantee is what matters.
    std::cout << std::boolalpha;
    std::cout << "high_resolution_clock guaranteed steady? no -- implementation-defined\n";

    // Fix: name steady_clock for intervals. Its monotonicity is guaranteed by the
    // standard, so now() never goes backwards.
    std::cout << "steady_clock::is_steady = " << steady_clock::is_steady << '\n';   // always true
    auto a = steady_clock::now();
    auto b = steady_clock::now();
    std::cout << "[correct] steady interval non-negative? " << (b >= a ? "yes" : "no") << '\n';
    return 0;
}
