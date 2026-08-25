#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: taking "today" as the UTC date of the current instant. Near
    // midnight the UTC date and the user's LOCAL date differ, so a "due today"
    // check or a daily boundary can be off by a day. (A fixed instant is used so
    // the output is reproducible; the real bug uses system_clock::now().)

    // 2026-08-21 23:30 in New York is already 2026-08-22 in UTC.
    zoned_time nyLateNight{"America/New_York", local_days{2026y / August / 21d} + 23h + 30min};
    sys_seconds asUtc = nyLateNight.get_sys_time();

    // WRONG: the UTC date.
    year_month_day utcDate = floor<days>(asUtc);
    std::cout << "UTC 'today':    " << std::format("{:%Y-%m-%d}", utcDate) << '\n';

    // RIGHT: the date in the relevant zone. A local_days is not a sys_days, so
    // take its day count to read out the calendar date.
    local_days localDays = floor<days>(nyLateNight.get_local_time());
    year_month_day localDate{sys_days{localDays.time_since_epoch()}};
    std::cout << "New York today: " << std::format("{:%Y-%m-%d}", localDate) << '\n';

    std::cout << "they differ: " << (utcDate != localDate) << '\n';
    return 0;
}
