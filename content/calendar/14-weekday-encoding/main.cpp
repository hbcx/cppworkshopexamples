#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: mixing weekday's two numberings. c_encoding() is 0..6 with
    // SUNDAY = 0 (the C / struct tm convention); iso_encoding() is 1..7 with
    // MONDAY = 1 .. SUNDAY = 7 (ISO 8601). They agree Monday..Friday and diverge
    // at the weekend, which is where the bug hides.
    weekday fri{sys_days{2026y / August / 21d}};
    std::cout << "Friday  c_encoding=" << fri.c_encoding()
              << " iso_encoding=" << fri.iso_encoding() << " (agree)\n";

    weekday sun = Sunday;
    std::cout << "Sunday  c_encoding=" << sun.c_encoding()
              << " iso_encoding=" << sun.iso_encoding() << " (differ)\n";

    // A Monday-first, 0-based index is iso_encoding() - 1, NOT c_encoding():
    std::cout << "Monday-first index of Sunday: " << (sun.iso_encoding() - 1) << '\n'; // 6
    std::cout << "Sunday-first index of Sunday: " << sun.c_encoding() << '\n';         // 0

    // RIGHT: build from a date or a named constant, and pick the encoding that
    // matches the week layout you want.
    return 0;
}
