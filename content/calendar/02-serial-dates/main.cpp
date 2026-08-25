#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    year_month_day date = 2026y / August / 21d;

    // sys_days is the date as a serial count of days since 1970-01-01. It is the
    // bridge between the calendar view and arithmetic.
    sys_days serial = date;
    std::cout << "days since 1970-01-01: " << serial.time_since_epoch().count() << '\n';

    // Day arithmetic happens on sys_days (always a real date), then convert back.
    year_month_day in10days = serial + days{10};
    std::cout << "10 days later: " << std::format("{:%Y-%m-%d}", in10days) << '\n';

    // Subtracting two dates gives the exact number of days between them.
    sys_days newYear = sys_days{2027y / January / 1d};
    days until = newYear - serial;
    std::cout << "days until 2027-01-01: " << until.count() << '\n';
    return 0;
}
