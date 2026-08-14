#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

int main() {
    std::cout << std::boolalpha;
    const double nan = std::numeric_limits<double>::quiet_NaN();

    // ANTI-PATTERN: trusting min/max/clamp to tame a floating-point value that
    // might be NaN. Every comparison involving NaN is FALSE (NaN < x, x < NaN,
    // NaN == NaN are all false), and these functions are built on <, so NaN
    // slips through in surprising, order-dependent ways. This is defined
    // behaviour, just not the behaviour people expect -- so we can run it.

    // clamp does NOT pull NaN into the range: it returns NaN, not a bound.
    double c = std::clamp(nan, 0.0, 1.0);
    std::cout << "clamp(NaN, 0, 1) is NaN? " << std::isnan(c) << '\n';

    // max is even worse: the result depends on ARGUMENT ORDER, because it just
    // returns the first argument when the comparison is false.
    std::cout << "max(NaN, 5) is NaN? " << std::isnan(std::max(nan, 5.0)) << '\n';
    std::cout << "max(5, NaN) = " << std::max(5.0, nan)
              << "   <- same call, different order, different answer\n";

    // FIX: reject or replace NaN explicitly BEFORE clamping, with std::isnan.
    double input = nan;
    double safe = std::isnan(input) ? 0.0 : std::clamp(input, 0.0, 1.0);
    std::cout << "guarded result = " << safe << '\n';
    return 0;
}
