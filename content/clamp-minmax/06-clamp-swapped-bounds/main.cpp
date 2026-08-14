#include <algorithm>
#include <iostream>
#include <utility>

int main() {
    int value = 42;

    // Bounds that come from data or a calculation, not literals -- so their order
    // is not guaranteed. Imagine these arrived swapped by mistake.
    int a = 100, b = 0;   // intended range is [0, 100], but a > b here

    // ANTI-PATTERN: std::clamp(value, a, b) when a > b.
    //
    //   int wrong = std::clamp(value, a, b);   // a=100 > b=0 -> UNDEFINED
    //
    // std::clamp REQUIRES lo <= hi. If you pass the bounds in the wrong order the
    // behaviour is undefined -- it may return either bound or the value, and the
    // standard makes no promise. This is easy to hit when lo and hi are computed.
    // We do not run that line.

    // FIX: order the bounds first. std::minmax gives {min, max} in one call, so
    // the clamp always gets lo <= hi regardless of how a and b arrived.
    auto bounds = std::minmax(a, b);
    int safe = std::clamp(value, bounds.first, bounds.second);
    std::cout << "ordered bounds: [" << bounds.first << ", " << bounds.second
              << "], clamp(42) = " << safe << '\n';

    // A value outside the range confirms the bounds now work as intended.
    std::cout << "clamp(250) = "
              << std::clamp(250, bounds.first, bounds.second) << '\n';
    std::cout << "clamp(-7)  = "
              << std::clamp(-7, bounds.first, bounds.second) << '\n';
    return 0;
}
