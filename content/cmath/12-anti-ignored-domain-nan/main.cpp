#include <cmath>
#include <iostream>
#include <iomanip>

// TRAP: calling a math function outside its domain and using the result without
// checking. sqrt of a negative, log of zero or negative, acos of an argument
// past +/-1 -- none of these throw. They return NaN (or -inf) and set errno,
// and a NaN then spreads silently through every later calculation, so a single
// bad input turns a whole result into "nan" with no obvious origin.
double triangleAngle(double a, double b, double c) {
    // Law of cosines. If a, b, c are not a valid triangle the argument to acos
    // can fall outside [-1, 1], and acos returns NaN.
    double cosA = (b*b + c*c - a*a) / (2 * b * c);
    return std::acos(cosA);
}

int main() {
    std::cout << std::setprecision(6) << std::boolalpha;

    double bad = std::sqrt(-1.0);          // NaN, no exception thrown
    // Print the CHECK, not the raw NaN (whose printed form and sign vary).
    std::cout << "isnan(sqrt(-1))   = " << std::isnan(bad)          << "\n";
    std::cout << "isinf(log(0))     = " << std::isinf(std::log(0.0))<< "   (log 0 is -inf)\n";

    // A degenerate triangle (sides 5, 1, 1 cannot close) drives acos out of its
    // [-1, 1] domain, so the angle comes back NaN.
    double angle = triangleAngle(5.0, 1.0, 1.0);
    std::cout << "isnan(bad angle)  = " << std::isnan(angle) << "\n";

    // FIX: validate the domain, or check the result before trusting it.
    if (std::isnan(angle))
        std::cout << "\ndetected: inputs are not a valid triangle -- do not use the angle\n";
    return 0;
}
