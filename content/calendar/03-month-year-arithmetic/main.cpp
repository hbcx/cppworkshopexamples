#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Months and years add straight to a year_month_day and are CALENDRICAL:
    // +1 month keeps the day number and moves to the next month.
    year_month_day date = 2026y / January / 31d;

    year_month_day nextMonth = date + months{1}; // 2026-02-31 does not exist
    std::cout << "Jan 31 + 1 month: " << std::format("{:%Y-%m-%d}", nextMonth)
              << " ok=" << nextMonth.ok() << '\n';

    // The fix: if it overflowed, snap to the last valid day of that month.
    year_month_day fixed =
        nextMonth.ok() ? nextMonth
                       : year_month_day{nextMonth.year() / nextMonth.month() / last};
    std::cout << "snapped to month end: " << std::format("{:%Y-%m-%d}", fixed) << '\n';

    // Years work the same; a leap day overflows on a common year.
    year_month_day leap = 2024y / February / 29d;
    year_month_day plusYear = leap + years{1};
    std::cout << "Feb 29 + 1 year: " << std::format("{:%Y-%m-%d}", plusYear)
              << " ok=" << plusYear.ok() << '\n';
    return 0;
}
