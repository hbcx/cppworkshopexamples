#include <limits>
#include <iostream>
#include <cstdint>

// TRAP: a hard-coded "largest int" baked into a template. Correct for 32-bit
// int, but silently wrong the moment T is wider -- the ceiling sits far below
// the type's real maximum.
template <class T>
T clamp_to_hardcoded_max(T v) {
    const T MAX = 2147483647;          // 0x7FFFFFFF -- assumes a 32-bit type!
    return v > MAX ? MAX : v;
}

// FIX: ask numeric_limits for the real maximum of T.
template <class T>
T clamp_to_real_max(T v) {
    const T MAX = std::numeric_limits<T>::max();
    return v > MAX ? MAX : v;
}

int main() {
    std::int64_t big = 5000000000LL;   // 5e9, fits easily in int64

    std::cout << "value                    = " << big << "\n";
    std::cout << "clamped (hard-coded max) = " << clamp_to_hardcoded_max(big)
              << "  (wrong: chopped to 2^31-1)\n";
    std::cout << "clamped (numeric_limits) = " << clamp_to_real_max(big)
              << "  (correct: unchanged, it fits)\n";
}
