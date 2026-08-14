#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::cout << std::boolalpha;

    std::vector<int> big{1, 2, 3, 4, 5};
    std::vector<int> small{1, 2, 3};   // shorter than big

    // ANTI-PATTERN: the three-argument std::equal / std::mismatch.
    //
    //   bool same = std::equal(big.begin(), big.end(), small.begin());
    //
    // This form reads as many elements from the second range as the FIRST range
    // has -- five here -- but small holds only three. It walks two elements past
    // small's end, which is undefined behaviour: it may read garbage, crash, or
    // appear to work. The three-argument form is only safe when you already know
    // the second range is at least as long as the first. We do not run that line.

    // FIX: the four-iterator overload (C++14) takes BOTH ranges in full. It
    // compares the lengths first and returns false when they differ -- no overrun.
    bool same = std::equal(big.begin(), big.end(), small.begin(), small.end());
    std::cout << "big equals small (safe four-arg)? " << same << '\n';

    // The same fix applies to mismatch: give it last2 so it stops at the shorter
    // end instead of running off small.
    auto d = std::mismatch(big.begin(), big.end(), small.begin(), small.end());
    std::cout << "compared safely up to index "
              << (d.second - small.begin()) << " of small\n";
    return 0;
}
