#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> h{5, 3, 4, 1};
    std::make_heap(h.begin(), h.end());
    std::cout << "top after make_heap: " << h.front() << '\n';

    // ADD: first push_back the new value, THEN std::push_heap, which sifts that
    // last element up into its correct place so the range is a heap again. Adding
    // 8 makes it the new maximum.
    h.push_back(8);
    std::push_heap(h.begin(), h.end());
    std::cout << "top after pushing 8: " << h.front() << '\n';

    // REMOVE THE TOP: std::pop_heap moves the front (the max) to the LAST
    // position and re-heapifies everything before it. It does NOT shrink the
    // container -- you then pop_back to actually drop that element.
    std::pop_heap(h.begin(), h.end());
    int removed = h.back();
    h.pop_back();
    std::cout << "removed top: " << removed << ", new top: " << h.front() << '\n';

    // Repeatedly pop_heap + pop_back yields the elements in decreasing order --
    // this is how a priority queue serves the largest first.
    std::cout << "draining largest-first:";
    while (!h.empty()) {
        std::pop_heap(h.begin(), h.end());
        std::cout << ' ' << h.back();
        h.pop_back();
    }
    std::cout << '\n';
    return 0;
}
