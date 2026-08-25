#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: using a date without checking ok(). An invalid year_month_day
    // does not throw; it just holds impossible fields, and converting it to
    // sys_days SILENTLY normalises to a different date.
    year_month_day date = 2026y / February / 30d; // does not exist

    std::cout << "date fields: " << std::format("{:%Y-%m-%d}", date) << '\n';
    std::cout << "ok(): " << date.ok() << '\n';

    // Convert the bad date to a serial day and back: Feb 30 rolls over into
    // March, with no error at all.
    year_month_day normalised = sys_days{date};
    std::cout << "as sys_days and back: " << std::format("{:%Y-%m-%d}", normalised)
              << " (not what was intended)\n";

    // RIGHT: check ok() before trusting a date.
    if (!date.ok())
        std::cout << "rejected: 2026-02-30 is not a real date\n";
    return 0;
}
