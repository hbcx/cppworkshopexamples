#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // A fixed instant in UTC: 2026-08-21 12:00 UTC.
    sys_seconds instant = sys_days{2026y / August / 21d} + 12h;
    std::cout << "UTC:      " << std::format("{:%Y-%m-%d %H:%M %Z}", instant) << '\n';

    // zoned_time pairs the instant with a named zone and gives the local time
    // there, daylight saving included.
    zoned_time newYork{"America/New_York", instant};
    zoned_time tokyo{"Asia/Tokyo", instant};
    std::cout << "New York: " << std::format("{:%Y-%m-%d %H:%M %Z}", newYork) << '\n';
    std::cout << "Tokyo:    " << std::format("{:%Y-%m-%d %H:%M %Z}", tokyo) << '\n';
    std::cout << "NY offset from UTC: " << std::format("{:%z}", newYork) << '\n';

    // The other direction: a LOCAL wall-clock time in a zone -> the UTC instant.
    zoned_time meeting{"Europe/London", local_days{2026y / August / 21d} + 9h};
    std::cout << "London 09:00 local = "
              << std::format("{:%H:%M}", meeting.get_sys_time()) << " UTC\n";
    return 0;
}
