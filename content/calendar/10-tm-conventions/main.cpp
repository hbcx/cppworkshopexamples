#include <chrono>
#include <ctime>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: treating chrono month/year like the fields of struct tm. In
    // <ctime>, months are 0-based (January = 0) and the year is an offset from
    // 1900. chrono is natural: January is month 1, the year is the real year.

    // A tm for "August 2026" as C requires it: month 7, year 126.
    std::tm t{};
    t.tm_year = 2026 - 1900; // 126
    t.tm_mon = 8 - 1;        // 7 for August
    t.tm_mday = 21;

    // WRONG: feeding the tm numbers straight into chrono types.
    year_month_day wrong = year{t.tm_year} / (t.tm_mon) / t.tm_mday;
    std::cout << "tm fields into chrono: " << std::format("{:%Y-%m-%d}", wrong)
              << " (year 126, month 7 -- wrong)\n";

    // RIGHT: chrono uses real, 1-based values.
    year_month_day right = year{2026} / 8 / 21;
    std::cout << "chrono values:        " << std::format("{:%Y-%m-%d}", right) << '\n';

    // Or convert tm's conventions explicitly at the boundary.
    year_month_day converted = year{t.tm_year + 1900} / (t.tm_mon + 1) / t.tm_mday;
    std::cout << "converted from tm:    " << std::format("{:%Y-%m-%d}", converted) << '\n';
    return 0;
}
