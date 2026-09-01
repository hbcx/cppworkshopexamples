#include <mdspan>
#include <vector>
#include <iostream>

int main() {
    // A buffer of only 10 elements.
    std::vector<int> v(10);
    for (int i = 0; i < 10; ++i) v[i] = i;   // 0..9

    // TRAP: we claim a 3x4 = 12-element grid over a 10-element buffer. mdspan
    // does NO size or bounds checking, so this is accepted silently, and
    // bad[2,3] (flat offset 11) would read two elements past the end -- that is
    // undefined behaviour. We do NOT run that access:
    std::mdspan bad(v.data(), 3, 4);
    std::cout << "bad claims size " << bad.size()
              << " over a buffer of " << v.size() << " -- no check happened\n";
    // std::cout << bad[2, 3];   // <-- undefined behaviour: offset 11 >= 10

    // FIX: make the extents match the real element count (2x5 = 10). Now every
    // index is in range.
    std::mdspan good(v.data(), 2, 5);
    std::cout << "good[1,4] = " << good[1, 4] << "\n";   // last element = 9
}
