#include <cstdint>
#include <iostream>
#include <limits>

// <cstdint> offers three families of signed/unsigned integers:
//   intN_t        exactly N bits (optional -- exists only where the platform
//                 has such a type). Use when the width must match a fixed
//                 external layout: a file, a wire format, a register.
//   int_leastN_t  the smallest type with AT LEAST N bits (always available).
//                 Use when you need a minimum range but not an exact size.
//   int_fastN_t   the fastest type with at least N bits (always available).
//                 Use for a loop counter or local math where speed beats size.
int main() {
    std::int32_t       exact = 0;
    std::int_least32_t least = 0;
    std::int_fast32_t  fast  = 0;
    (void)exact; (void)least; (void)fast;

    std::cout << "want at least 32 bits of range:\n";
    std::cout << "int32_t        : sizeof " << sizeof(std::int32_t)
              << ", " << std::numeric_limits<std::int32_t>::digits << " value bits\n";
    std::cout << "int_least32_t  : sizeof " << sizeof(std::int_least32_t)
              << ", " << std::numeric_limits<std::int_least32_t>::digits << " value bits\n";
    std::cout << "int_fast32_t   : sizeof " << sizeof(std::int_fast32_t)
              << ", " << std::numeric_limits<std::int_fast32_t>::digits << " value bits\n";

    std::cout << "\nrule of thumb:\n";
    std::cout << "  exact layout on disk / wire -> intN_t\n";
    std::cout << "  just need the range         -> int_leastN_t\n";
    std::cout << "  hot loop counter            -> int_fastN_t\n";
}
