#include <limits>
#include <iostream>
#include <cstdint>

int main() {
    std::cout << std::boolalpha;

    // TRAP: adding to the maximum of a SIGNED integer overflows, which is
    // undefined behaviour -- NOT a wrap to the minimum. We do NOT run it:
    //     int x = std::numeric_limits<int>::max();
    //     int y = x + 1;   // signed overflow: undefined behaviour

    int a = std::numeric_limits<int>::max();
    std::cout << "int max = " << a << "\n";

    // FIX 1: test against numeric_limits BEFORE the operation, so the
    // overflowing addition never happens.
    bool would_overflow = (a > std::numeric_limits<int>::max() - 1);
    std::cout << "adding 1 to int max would overflow? " << would_overflow << "\n";  // true

    // FIX 2: compute in a wider type where the value fits.
    std::int64_t wide = static_cast<std::int64_t>(a) + 1;
    std::cout << "in int64: max + 1 = " << wide << "\n";                            // 2147483648
}
