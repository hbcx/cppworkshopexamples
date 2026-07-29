#include <chrono>
#include <ctime>
#include <iostream>

int main() {
    using namespace std::chrono;

    // A FIXED instant so the output is reproducible: 1 000 000 000 seconds after
    // the Unix epoch. Building a system_clock time_point from a known time_t.
    std::time_t fixed = 1000000000;                     // 2001-09-09 01:46:40 UTC
    system_clock::time_point tp = system_clock::from_time_t(fixed);

    // Round-trip back to time_t proves the mapping.
    std::time_t back = system_clock::to_time_t(tp);
    std::cout << "round-trips through time_t? " << (back == fixed ? "yes" : "no") << '\n';

    // Break it into a UTC calendar via gmtime, then format with strftime.
    std::tm utc = *std::gmtime(&fixed);                 // UTC, so no timezone in the output
    char buf[32];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &utc);
    std::cout << "formatted (UTC): " << buf << '\n';    // 2001-09-09 01:46:40

    // Advancing the instant is duration arithmetic on the time_point.
    std::time_t plusDay = system_clock::to_time_t(tp + hours(24));
    std::cout << "one day later time_t = " << plusDay << " (== fixed + 86400? "
              << (plusDay == fixed + 86400 ? "yes" : "no") << ")\n";
    return 0;
}
