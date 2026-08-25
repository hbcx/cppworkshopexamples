#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // A civil date is a year_month_day, built with the / operator. Two spellings:
    year_month_day a = 2026y / August / 21d; // calendar literals
    year_month_day b = year{2026} / 8 / 21;  // plain values

    std::cout << "a: " << std::format("{:%Y-%m-%d}", a) << '\n';
    std::cout << "b: " << std::format("{:%Y-%m-%d}", b) << '\n';
    std::cout << "same date: " << (a == b) << '\n';

    // The parts are strong types, not ints; convert out when you need a number.
    std::cout << "year " << int(a.year()) << ", month " << unsigned(a.month())
              << ", day " << unsigned(a.day()) << '\n';

    // ok() reports whether the date actually exists -- construction never checks.
    std::cout << "a.ok(): " << a.ok() << '\n';
    year_month_day bad = 2026y / February / 30d;
    std::cout << "2026-02-30 ok(): " << bad.ok() << '\n';
    return 0;
}
