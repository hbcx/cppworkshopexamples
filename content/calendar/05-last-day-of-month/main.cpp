#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // year/month/last is the last day of that month -- the leap rule is applied
    // for you, no month-length table needed.
    year_month_day_last febLeap = 2024y / February / last;  // leap year
    year_month_day_last febCommon = 2026y / February / last; // common year
    std::cout << "last of Feb 2024: " << std::format("{:%Y-%m-%d}", year_month_day{febLeap}) << '\n';
    std::cout << "last of Feb 2026: " << std::format("{:%Y-%m-%d}", year_month_day{febCommon}) << '\n';

    // The day() of the 'last' is the number of days in the month.
    std::cout << "days in Feb 2024: " << unsigned(febLeap.day()) << '\n';
    std::cout << "days in Feb 2026: " << unsigned(febCommon.day()) << '\n';

    // The leap-year question directly.
    std::cout << "2024 is leap: " << year{2024}.is_leap() << '\n';
    std::cout << "2026 is leap: " << year{2026}.is_leap() << '\n';
    return 0;
}
