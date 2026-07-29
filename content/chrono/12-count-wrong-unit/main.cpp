#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Same amount of time, different .count() -- the unit is invisible here.
    hours   oneHour(1);
    minutes sameInMin = oneHour;                 // 60 minutes, exactly the same span
    std::cout << "oneHour.count()   = " << oneHour.count()   << "  (hours)\n";     // 1
    std::cout << "sameInMin.count() = " << sameInMin.count() << "  (minutes)\n";   // 60

    // The bug: read .count() on the hours value while assuming minutes.
    long minutesWrong = oneHour.count();         // thinks 60, actually 1
    std::cout << "[buggy] minutes from oneHour.count() = " << minutesWrong << " (wanted 60)\n";

    // The fix: duration_cast to the unit you want BEFORE calling .count().
    long minutesRight = duration_cast<minutes>(oneHour).count();
    std::cout << "[correct] duration_cast<minutes>(oneHour).count() = " << minutesRight << '\n';   // 60
    return 0;
}
