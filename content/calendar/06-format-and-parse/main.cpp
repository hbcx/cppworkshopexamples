#include <chrono>
#include <format>
#include <iostream>
#include <sstream>

int main() {
    using namespace std::chrono;

    year_month_day date = 2026y / August / 21d;

    // Formatting: std::format understands the chrono date specifiers. Weekday and
    // month names need a sys_days (it carries the weekday); the numeric form does
    // not.
    std::cout << "ISO:       " << std::format("{:%Y-%m-%d}", date) << '\n';
    std::cout << "written:   " << std::format("{:%A, %B %d, %Y}", sys_days{date}) << '\n';
    std::cout << "day-first: " << std::format("{:%d/%m/%Y}", date) << '\n';

    // Parsing: read a date from text with chrono::parse via a stream.
    std::istringstream in{"2026-12-25"};
    year_month_day parsed;
    in >> parse("%Y-%m-%d", parsed);
    std::cout << "parsed ok: " << (!in.fail()) << " -> "
              << std::format("{:%Y-%m-%d}", parsed) << '\n';

    // Text that does not match the format sets the stream's fail bit.
    std::istringstream bad{"not-a-date"};
    year_month_day none;
    bad >> parse("%Y-%m-%d", none);
    std::cout << "bad input failed: " << bad.fail() << '\n';
    return 0;
}
