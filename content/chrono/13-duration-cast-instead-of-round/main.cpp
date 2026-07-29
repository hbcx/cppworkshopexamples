#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // 1900 ms remaining, shown to the user in whole seconds.
    milliseconds remaining(1900);

    // Anti-pattern: duration_cast truncates toward zero, so "1.9s left" reads as
    // "1s left" -- a countdown that under-reports.
    std::cout << "[buggy]   duration_cast: " << duration_cast<seconds>(remaining).count() << " s left\n";  // 1

    // Fix: a remaining-time display that must not under-report rounds UP.
    std::cout << "[correct] ceil:          " << ceil<seconds>(remaining).count() << " s left\n";   // 2
    // Or nearest, for a value that should read naturally.
    std::cout << "[correct] round:         " << round<seconds>(remaining).count() << " s\n";        // 2

    // Worst near zero: 400 ms truncates to 0 s, so the timer says "0 s left" with
    // nearly half a second to go.
    std::cout << "[buggy]   400ms truncates to " << duration_cast<seconds>(milliseconds(400)).count() << " s\n";  // 0
    std::cout << "[correct] 400ms ceils to     " << ceil<seconds>(milliseconds(400)).count() << " s\n";           // 1
    return 0;
}
