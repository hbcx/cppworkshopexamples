#include <iostream>
#include <compare>
#include <concepts>
#include <limits>

// The three ordering categories, shown by the type each comparison returns and by
// how they behave. The key difference is completeness (can two values be
// unordered?) and strictness (does equal mean identical?).

int main() {
    // Integers give strong_ordering: always exactly one of less/equal/greater,
    // and equal means truly interchangeable.
    static_assert(std::same_as<decltype(1 <=> 2), std::strong_ordering>);
    std::cout << "int  1 <=> 2 is less?    " << ((1 <=> 2) < 0) << "\n";   // 1

    // Floating point gives partial_ordering: a value can be UNORDERED with
    // another (NaN), so a comparison may be none of less, equal, or greater.
    static_assert(std::same_as<decltype(1.0 <=> 2.0), std::partial_ordering>);
    double nan = std::numeric_limits<double>::quiet_NaN();
    auto r = (1.0 <=> nan);
    std::cout << "dbl  1.0 <=> NaN unordered? "
              << (r == std::partial_ordering::unordered) << "\n";          // 1
    std::cout << "  and 1.0 < NaN?  " << (1.0 < nan)
              << "   1.0 == NaN? " << (1.0 == nan) << "\n";                 // 0   0

    // weak_ordering is the middle case -- total like strong, but "equal" means
    // only equivalent, not identical. It is shown in the weak-ordering example
    // (case-insensitive text: "Hello" and "hello" order as equivalent yet differ).
    std::cout << "(weak_ordering: see the case-insensitive example)\n";
    return 0;
}
