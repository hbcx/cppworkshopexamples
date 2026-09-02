#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    // Large values whose running sum exceeds what a 32-bit int can hold.
    std::vector<int> v{1000000000, 1000000000, 1000000000, 1000000000};

    // BAD: partial_sum accumulates into the OUTPUT element type. With an int output
    // the running sum overflows once it passes about 2.1 billion -- undefined
    // behavior for signed int, and a wrong (wrapped) result. Shown as a comment:
    //   std::vector<int> bad;
    //   std::partial_sum(v.begin(), v.end(), std::back_inserter(bad));  // overflow

    // GOOD: use a wide-enough element type for the running sum. Copy into a
    // long long range (or hold the data as long long), so the sums fit.
    std::vector<long long> wide(v.begin(), v.end());
    std::vector<long long> sums;
    std::partial_sum(wide.begin(), wide.end(), std::back_inserter(sums));

    std::cout << "prefix sums (long long): ";
    for (long long x : sums) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
