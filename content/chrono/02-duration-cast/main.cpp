#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Widening loses nothing, so it is implicit: seconds -> milliseconds.
    seconds      twoSec(2);
    milliseconds asMs = twoSec;                 // no cast needed
    std::cout << "2s as ms = " << asMs.count() << '\n';   // 2000

    // Narrowing can lose the fraction, so it must be an explicit duration_cast,
    // and the cast truncates toward zero.
    milliseconds oneAndHalf(1500);
    seconds truncated = duration_cast<seconds>(oneAndHalf);
    std::cout << "1500ms as s (truncated) = " << truncated.count() << '\n';   // 1

    // Truncation is toward zero, NOT floor: -1500ms becomes -1s, not -2s.
    milliseconds negHalf(-1500);
    std::cout << "-1500ms as s (toward zero) = "
              << duration_cast<seconds>(negHalf).count() << '\n';   // -1

    // Keep the fraction by casting to a floating-point duration instead.
    duration<double> fractional = duration_cast<duration<double>>(oneAndHalf);
    std::cout << "1500ms as seconds (double) = " << fractional.count() << '\n';   // 1.5
    return 0;
}
