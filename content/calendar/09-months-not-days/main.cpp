#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: using month arithmetic when the requirement is a fixed number
    // of days (or the reverse). "One month later" and "30 days later" differ, and
    // month arithmetic can produce an invalid date.
    year_month_day start = 2026y / January / 31d;

    // A "30-day window" done WRONG with months: keeps the day (31) -> invalid.
    year_month_day byMonth = start + months{1};
    std::cout << "start + months{1}: " << std::format("{:%Y-%m-%d}", byMonth)
              << " ok=" << byMonth.ok() << '\n';

    // A "30-day window" done RIGHT: day arithmetic through sys_days.
    year_month_day byDays = sys_days{start} + days{30};
    std::cout << "start + days{30}:  " << std::format("{:%Y-%m-%d}", byDays)
              << " ok=" << byDays.ok() << '\n';

    // Use months/years for calendar steps (same day next month), and
    // sys_days + days for an exact number of days that can never overflow.
    return 0;
}
