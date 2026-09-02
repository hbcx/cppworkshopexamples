#include <cmath>      // std::abs(double), std::fabs
#include <iostream>
#include <iomanip>

// TRAP: calling the integer abs on a floating value. The C library <cstdlib>
// declares int abs(int); if that overload is the one in scope (a bare abs after
// including <cstdlib>, or ::abs), a double argument is CONVERTED TO int first --
// so abs(-3.7) truncates to -3 and returns 3, silently losing the fraction.
//
//     #include <cstdlib>
//     double bad = abs(-3.7);   // -3.7 -> int -3 -> abs 3   (fraction gone!)
//
// The line above is shown only as a comment: modern compilers flag it with
// -Wabsolute-value, which -Werror turns into a hard error. We run the fixes.
int main() {
    std::cout << std::fixed << std::setprecision(2);

    double v = -3.7;

    // std::abs has a floating overload (from <cmath>), and std::fabs is the
    // explicitly-floating name. Both keep the fraction.
    std::cout << "std::abs(-3.7)  = " << std::abs(v)  << "\n";
    std::cout << "std::fabs(-3.7) = " << std::fabs(v) << "\n";

    std::cout << "\nFIX: use std::abs (has a double overload) or std::fabs on\n"
                 "floating values; the bare/integer abs would truncate to 3.\n";
    return 0;
}
