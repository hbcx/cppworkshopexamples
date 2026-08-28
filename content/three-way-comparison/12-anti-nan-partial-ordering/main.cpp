#include <iostream>
#include <vector>
#include <limits>

// Anti-pattern: writing order-dependent logic that assumes any two values compare.
// Floating point breaks that assumption: NaN is unordered, so a naive maximum can
// get stuck on it, and NaN is not even equal to itself.

// Naive running maximum: "replace best whenever best < x". Fine until best is NaN,
// which is less than nothing, so it is never replaced.
double naiveMax(const std::vector<double>& v) {
    double best = v.front();
    for (std::size_t i = 1; i < v.size(); ++i)
        if (best < v[i]) best = v[i];
    return best;
}

// Fixed: skip the values that are not ordered (NaN fails x == x).
double safeMax(const std::vector<double>& v) {
    double best = -std::numeric_limits<double>::infinity();
    for (double x : v)
        if (x == x && best < x) best = x;   // x == x is false only for NaN
    return best;
}

int main() {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    std::vector<double> data{nan, 2.0, 5.0};   // NaN happens to be first

    // The naive version starts on NaN and never moves off it:
    std::cout << "naiveMax = " << naiveMax(data) << " (expected 5)\n";   // nan
    std::cout << "safeMax  = " << safeMax(data)  << "\n";                // 5

    // NaN is not equal to itself, so equality-based checks miss it entirely:
    std::cout << "nan == nan? " << (nan == nan) << "\n";                 // 0
    return 0;
}
