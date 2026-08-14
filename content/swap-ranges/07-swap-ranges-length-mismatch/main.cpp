#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 4, 5};   // 5 elements
    std::vector<int> b{10, 20};          // only 2

    // BAD: swap_ranges reads the SECOND range for the SAME length as the first,
    // with no bounds check. With the first range 5 long and the second only 2, it
    // walks past b's end -- undefined behavior. Shown only as a comment:
    //   std::swap_ranges(a.begin(), a.end(), b.begin());   // reads 5 from a 2-vector

    // GOOD: drive the swap by the SHORTER length so the second range is never
    // overrun -- swap only as many as the smaller range holds.
    std::size_t n = std::min(a.size(), b.size());
    std::swap_ranges(a.begin(), a.begin() + n, b.begin());

    std::cout << "a: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << "\nb: ";
    for (int x : b) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
