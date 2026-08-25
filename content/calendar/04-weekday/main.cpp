#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // The weekday of a date comes from its sys_days.
    sys_days date = sys_days{2026y / August / 21d};
    weekday wd{date};
    std::cout << "2026-08-21 is a " << std::format("{:%A}", wd) << '\n';
    std::cout << "iso (Mon=1..Sun=7): " << wd.iso_encoding() << '\n';

    // "Is it a weekend?" -- compare against the named constants.
    bool weekend = (wd == Saturday || wd == Sunday);
    std::cout << "weekend: " << weekend << '\n';

    // Indexed weekdays: the 2nd Thursday of August 2026.
    year_month_weekday secondThu = 2026y / August / Thursday[2];
    std::cout << "2nd Thursday of Aug 2026: "
              << std::format("{:%Y-%m-%d}", sys_days{secondThu}) << '\n';

    // The last Friday of the month.
    year_month_weekday_last lastFri = 2026y / August / Friday[last];
    std::cout << "last Friday of Aug 2026: "
              << std::format("{:%Y-%m-%d}", sys_days{lastFri}) << '\n';
    return 0;
}
