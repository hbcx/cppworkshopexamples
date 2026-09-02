#include <limits>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

int main() {
    std::cout << std::boolalpha;
    using lim = std::numeric_limits<double>;

    // Three counts, easily confused:
    //   digits       = mantissa bits (radix-2 precision)
    //   digits10     = decimal digits that always survive a round-trip TO text
    //   max_digits10 = decimal digits needed to round-trip a value back exactly
    std::cout << "double: digits=" << lim::digits
              << " digits10=" << lim::digits10
              << " max_digits10=" << lim::max_digits10 << "\n";   // 53, 15, 17

    // Printing with digits10 (15) can lose the exact value; max_digits10 (17)
    // guarantees the text reads back as the SAME double.
    double v = 0.1 + 0.2;
    std::ostringstream few, enough;
    few    << std::setprecision(lim::digits10)     << v;
    enough << std::setprecision(lim::max_digits10) << v;
    std::cout << "with digits10:     " << few.str() << "\n";      // 0.3
    std::cout << "with max_digits10: " << enough.str() << "\n";   // 0.30000000000000004

    double back = std::stod(enough.str());
    std::cout << "max_digits10 round-trips exactly? " << (back == v) << "\n";  // true
}
