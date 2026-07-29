#include <chrono>
#include <iostream>
#include <thread>

int main() {
    using namespace std::chrono;

    // The stopwatch: read the clock, do the work, read again, subtract.
    auto start = steady_clock::now();
    std::this_thread::sleep_for(milliseconds(10));   // stand-in for real work
    auto elapsed = steady_clock::now() - start;      // a steady_clock::duration

    // Report by casting at the point of printing. We print the RELATIONSHIP, not
    // the raw nanoseconds, which vary from run to run.
    auto ms = duration_cast<milliseconds>(elapsed);
    std::cout << "slept for at least 10ms? " << (ms >= milliseconds(10) ? "yes" : "no") << '\n';
    std::cout << "elapsed is non-negative? " << (elapsed >= steady_clock::duration::zero() ? "yes" : "no") << '\n';
    std::cout << "measured with a steady clock? " << std::boolalpha << steady_clock::is_steady << '\n';
    return 0;
}
