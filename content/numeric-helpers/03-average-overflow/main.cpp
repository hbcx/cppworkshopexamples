#include <cstdint>
#include <iostream>
#include <numeric>

int main() {
    // Two large 32-bit unsigned values. Their true average is 3,500,000,000,
    // which fits in the type -- the problem is only the intermediate sum.
    std::uint32_t a = 3000000000u;
    std::uint32_t b = 4000000000u;

    // ANTI-PATTERN: average with (a + b) / 2. The SUM a + b is 7,000,000,000,
    // which does not fit in uint32_t, so it wraps around (modulo 2^32) BEFORE the
    // division. The result is a small, wrong number even though the real average
    // fits fine. (For signed types the same overflow would be undefined behaviour;
    // here it is defined wraparound, so we can show it.)
    std::uint32_t naive = (a + b) / 2;
    std::cout << "naive (a + b) / 2 = " << naive << "   <- wrong, sum overflowed\n";

    // FIX: std::midpoint never forms a + b, so it cannot overflow. It computes the
    // halfway value directly and gives the correct answer.
    std::uint32_t mid = std::midpoint(a, b);
    std::cout << "std::midpoint(a, b) = " << mid << '\n';

    // The classic place this bug hides is binary search: mid = (low + high) / 2
    // overflows once the indices get large. midpoint (or low + (high - low) / 2)
    // is the safe form.
    return 0;
}
