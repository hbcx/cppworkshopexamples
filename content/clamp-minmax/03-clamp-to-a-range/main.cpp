#include <algorithm>
#include <iostream>

int main() {
    const int lo = 0, hi = 100;   // a valid range, e.g. a volume percentage

    // std::clamp(v, lo, hi) pins v into [lo, hi]: below lo it returns lo, above
    // hi it returns hi, otherwise v unchanged. One call, and the intent is
    // obvious -- exactly what you want for bounding user or sensor input.
    for (int v : {-20, 0, 42, 100, 137}) {
        std::cout << "clamp(" << v << ", 0, 100) = "
                  << std::clamp(v, lo, hi) << '\n';
    }

    // The pre-C++17 way was std::max(lo, std::min(v, hi)) -- correct, but easy
    // to get backwards (swap min and max and it silently does the wrong thing).
    // clamp says what it means and reads in the natural order low..high.
    int v = 137;
    std::cout << "old form max(lo,min(v,hi)) = "
              << std::max(lo, std::min(v, hi)) << '\n';
    return 0;
}
