#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: hardcoding a zone offset. A zone's offset changes with
    // daylight saving, so a fixed number is right for half the year and wrong for
    // the other half. Let the tz database decide.

    sys_seconds summer = sys_days{2026y / July / 1d} + 12h;    // July: DST in New York
    sys_seconds winter = sys_days{2026y / January / 1d} + 12h; // January: standard time

    zoned_time nySummer{"America/New_York", summer};
    zoned_time nyWinter{"America/New_York", winter};

    std::cout << "New York, July: " << std::format("{:%H:%M %Z (%z)}", nySummer) << '\n';
    std::cout << "New York, Jan:  " << std::format("{:%H:%M %Z (%z)}", nyWinter) << '\n';

    // "New York is UTC-5" is only true in winter; in summer it is UTC-4, so a
    // hardcoded -5 would put every summer timestamp an hour off.
    bool sameAllYear = nySummer.get_info().offset == nyWinter.get_info().offset;
    std::cout << "same offset all year? " << (sameAllYear ? "yes" : "no") << '\n';
    return 0;
}
