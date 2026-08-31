// Anti-pattern: trying to reserve() capacity on a deque.
//
// A deque grows by adding fixed-size chunks, never by reallocating one block, so
// it has no reserve() and no capacity(). resize() exists but it CONSTRUCTS
// elements -- it is not vector's empty pre-allocation.

#include <deque>
#include <iostream>

int main() {
    std::deque<int> d;

    // BAD (does not compile -- described):
    //   d.reserve(100);    // no such member: nothing to pre-allocate
    //   d.capacity();      // no such member either

    // A deque just grows; push_back is O(1) with no reallocation to avoid.
    for (int i = 0; i < 5; ++i) d.push_back(i);
    std::cout << "grew by push_back, size = " << d.size() << '\n';

    // If you really want N elements up front, resize CONSTRUCTS them
    // (value-initialized), which is not the same as reserving empty space.
    std::deque<int> pre;
    pre.resize(4);         // now holds 4 zeros -- real objects, not capacity
    std::cout << "after resize(4), size = " << pre.size() << ", contents:";
    for (int x : pre) std::cout << ' ' << x;
    std::cout << '\n';
}
