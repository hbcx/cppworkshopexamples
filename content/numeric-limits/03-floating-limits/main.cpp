#include <limits>
#include <iostream>

int main() {
    std::cout << std::boolalpha;
    using lim = std::numeric_limits<double>;

    // Capability flags: does the type support these specials at all?
    std::cout << "has_infinity=" << lim::has_infinity
              << " has_quiet_NaN=" << lim::has_quiet_NaN << "\n";

    double inf = lim::infinity();
    double nan = lim::quiet_NaN();
    std::cout << "infinity() = " << inf << ", 1.0/infinity() = " << (1.0 / inf) << "\n";
    std::cout << "quiet_NaN() = " << nan << "\n";

    // epsilon: the gap between 1.0 and the next representable double -- the unit
    // of relative precision.
    std::cout << "epsilon() = " << lim::epsilon() << "\n";

    // denorm_min is the smallest positive value at all (a subnormal), far below
    // min(), which is the smallest NORMAL value.
    std::cout << "min() = " << lim::min()
              << ", denorm_min() = " << lim::denorm_min() << "\n";
}
