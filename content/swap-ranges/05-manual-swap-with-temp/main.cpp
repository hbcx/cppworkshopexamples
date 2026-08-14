#include <utility>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a(1000, 1);   // a big buffer
    std::vector<int> b(1000, 2);

    // BAD: hand-rolling a swap with a temporary COPY. For a vector this makes THREE
    // full copies of a thousand elements -- O(n) work and two extra allocations --
    // to do what a swap should do in O(1).
    std::vector<int> tmp = a;   // copy 1
    a = b;                      // copy 2
    b = tmp;                    // copy 3
    std::cout << "manual swap done (3 copies of 1000 elements)\n";

    // GOOD: std::swap (or the member c.swap(d)) exchanges the vectors' internal
    // pointers in constant time, moving no elements at all.
    std::vector<int> c(1000, 3);
    std::vector<int> d(1000, 4);
    std::swap(c, d);            // O(1): swaps buffer pointers, size, capacity
    std::cout << "std::swap done (O(1), no element copies): c[0]=" << c[0]
              << " d[0]=" << d[0] << '\n';
    return 0;
}
