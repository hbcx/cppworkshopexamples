#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

static void print(const char* label, const std::vector<double>& v) {
    std::cout << label;
    for (double x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    // The 0.3 here is COMPUTED, so it is not bit-for-bit equal to the literal 0.3.
    std::vector<double> data{0.1 + 0.2, 0.5, nan, 0.9};

    // ANTI-PATTERN: std::replace uses ==, which for floating-point means EXACT
    // equality. Replacing the literal 0.3 misses the computed 0.1 + 0.2, and
    // replacing NaN never matches anything, because NaN == NaN is false.
    std::vector<double> a = data;
    std::replace(a.begin(), a.end(), 0.3, -1.0);        // does not match 0.1+0.2
    std::replace(a.begin(), a.end(), nan, -1.0);        // never matches the NaN
    print("after exact replace:", a);                   // unchanged where it matters

    // FIX: use replace_if with a predicate. A tolerance test catches near-values,
    // and std::isnan catches the NaN that == can never find.
    std::vector<double> b = data;
    std::replace_if(b.begin(), b.end(),
                    [](double x) { return std::fabs(x - 0.3) < 1e-9; }, -1.0);
    std::replace_if(b.begin(), b.end(),
                    [](double x) { return std::isnan(x); }, -1.0);
    print("after replace_if:   ", b);
    return 0;
}
