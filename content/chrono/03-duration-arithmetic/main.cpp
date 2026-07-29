#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Mixed units: the result is the finer unit (milliseconds), no manual scaling.
    auto total = seconds(2) + milliseconds(500);
    std::cout << "2s + 500ms = " << total.count() << " ms\n";   // 2500

    // Comparison across units converts first.
    std::cout << "1s > 999ms? " << (seconds(1) > milliseconds(999) ? "yes" : "no") << '\n';

    // duration / duration = a plain ratio (how many fit); duration / scalar = duration.
    auto ratio = minutes(2) / seconds(30);      // how many 30s in 2min
    std::cout << "2min / 30s = " << ratio << '\n';   // 4
    // Divide by a scalar keeps the unit, so widen to seconds FIRST -- minutes(2)/4
    // would be integer minutes(0), not 30 seconds.
    seconds twoMinInSec = minutes(2);           // 120s (exact widening)
    auto each = twoMinInSec / 4;                 // seconds(30)
    std::cout << "2min / 4 = " << each.count() << " s\n";   // 30

    // Integer durations divide with truncation: 2500ms / 1000 stays milliseconds.
    auto trunc = milliseconds(2500) / 1000;
    std::cout << "2500ms / 1000 = " << trunc.count() << " ms (still ms, truncated)\n";   // 2
    return 0;
}
