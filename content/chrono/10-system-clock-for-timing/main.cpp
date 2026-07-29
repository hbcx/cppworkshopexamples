#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // A real measurement subtracts two now() reads. With system_clock, an NTP
    // correction or DST change between them can move the clock BACKWARDS, so the
    // end can be earlier than the start. We cannot force a real adjustment here,
    // so we construct the two instants by hand to show what the subtraction does
    // when the wall clock was set back 2s mid-measurement.
    system_clock::time_point start = system_clock::from_time_t(1000000000);
    system_clock::time_point end   = start - seconds(2);   // clock jumped backwards

    auto elapsed = end - start;
    std::cout << "[buggy] system_clock elapsed = "
              << duration_cast<seconds>(elapsed).count() << " s (negative!)\n";
    std::cout << std::boolalpha << "system_clock::is_steady = " << system_clock::is_steady << '\n';

    // steady_clock cannot go backwards by definition, so a real interval measured
    // with it is always non-negative.
    auto a = steady_clock::now();
    auto b = steady_clock::now();
    std::cout << "steady_clock::is_steady = " << steady_clock::is_steady << '\n';   // true
    std::cout << "[correct] steady interval non-negative? "
              << (b - a >= steady_clock::duration::zero() ? "yes" : "no") << '\n';
    return 0;
}
