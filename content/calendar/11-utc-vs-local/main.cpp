#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Anti-pattern: confusing a UTC instant (sys_time) with a local wall-clock
    // time (local_time). They are different types on purpose; relabelling one as
    // the other shifts everything by the zone offset.

    // An instant: 2026-08-21 12:00 UTC.
    sys_seconds utc = sys_days{2026y / August / 21d} + 12h;
    zoned_time berlin{"Europe/Berlin", utc};

    // The SAME instant, two correct views:
    std::cout << "UTC time:    " << std::format("{:%H:%M %Z}", utc) << '\n';
    std::cout << "Berlin time: " << std::format("{:%H:%M %Z}", berlin) << '\n';

    // WRONG: take Berlin's wall-clock reading and relabel it as UTC. The numbers
    // (14:00) are now attached to the wrong instant, off by the offset.
    local_seconds wall = berlin.get_local_time();
    sys_seconds mistaken{wall.time_since_epoch()}; // reinterpret local as UTC
    std::cout << "Berlin wall read as UTC: " << std::format("{:%H:%M}", mistaken)
              << " -- off by the offset\n";

    // RIGHT: convert through the zone; never relabel the type.
    std::cout << "correct UTC again: " << std::format("{:%H:%M}", berlin.get_sys_time()) << '\n';
    return 0;
}
