#include <limits>
#include <iostream>
#include <vector>

// A generic maximum finder: seed the running max with the LOWEST value for T,
// so any real element is larger. lowest() works for integer and floating types
// alike (min() would be wrong for floats -- see the trap).
template <class T>
T max_of(const std::vector<T>& xs) {
    T best = std::numeric_limits<T>::lowest();
    for (const T& x : xs) if (x > best) best = x;
    return best;
}

// Would a value fit in a narrower type? Ask numeric_limits instead of guessing.
template <class To, class From>
bool fits(From v) {
    return v >= static_cast<From>(std::numeric_limits<To>::min())
        && v <= static_cast<From>(std::numeric_limits<To>::max());
}

int main() {
    std::cout << "max_of(ints)    = " << max_of<int>({-5, 3, -1, 8, 2}) << "\n";     // 8
    std::cout << "max_of(doubles) = " << max_of<double>({-2.5, -9.1, -0.3}) << "\n"; // -0.3

    std::cout << std::boolalpha;
    std::cout << "300 fits in unsigned char? " << fits<unsigned char>(300) << "\n";  // false
    std::cout << "200 fits in unsigned char? " << fits<unsigned char>(200) << "\n";  // true
}
