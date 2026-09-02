#include <cmath>
#include <iostream>

// TRAP: turning a double into an int with a plain cast when you meant to round.
// A cast to int TRUNCATES toward zero -- it drops the fraction -- so 2.9 becomes
// 2 and -2.9 becomes -2. For a price, a pixel, or a count that should be the
// NEAREST integer, that is a systematic downward bias and an off-by-one.
int main() {
    const double vals[] = { 2.4, 2.5, 2.9, -2.4, -2.9 };

    std::cout << "value   (int)cast   lround\n";
    for (double v : vals) {
        int truncated = static_cast<int>(v);   // WRONG when you want nearest
        long rounded  = std::lround(v);         // nearest, ties away from zero
        std::cout << "  " << v << "        " << truncated
                  << "          " << rounded << "\n";
    }

    std::cout << "\nstatic_cast<int>(2.9) is 2, not 3: the cast truncates.\n"
                 "FIX: std::lround / std::llround give the nearest integer type\n"
                 "directly; std::round keeps it as a double.\n";
    return 0;
}
