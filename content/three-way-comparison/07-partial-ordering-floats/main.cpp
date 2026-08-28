#include <iostream>
#include <compare>
#include <concepts>
#include <limits>

// A measurement wraps a double. Because double is not totally ordered (NaN is
// unordered), the defaulted <=> is partial_ordering -- and that shows up the
// moment a NaN is involved.

struct Measurement {
    double value;
    auto operator<=>(const Measurement&) const = default;
    bool operator==(const Measurement&) const = default;
};

int main() {
    // The category is partial_ordering, deduced from the double member:
    static_assert(std::same_as<decltype(Measurement{} <=> Measurement{}),
                               std::partial_ordering>);

    Measurement a{21.5};
    Measurement b{30.0};
    Measurement nan{std::numeric_limits<double>::quiet_NaN()};

    // Ordinary values order normally:
    std::cout << "a < b? " << (a < b) << "\n";   // 1

    // Against a NaN, every comparison is false -- the values are unordered:
    std::cout << "a <  nan? " << (a < nan)  << "\n";   // 0
    std::cout << "a >  nan? " << (a > nan)  << "\n";   // 0
    std::cout << "a == nan? " << (a == nan) << "\n";   // 0

    // The <=> result names it directly:
    std::cout << "a <=> nan unordered? "
              << ((a <=> nan) == std::partial_ordering::unordered) << "\n";   // 1
    return 0;
}
