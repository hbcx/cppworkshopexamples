#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::cout << std::boolalpha;
    std::vector<int> v{9, 6, 4, 3, 1};
    std::make_heap(v.begin(), v.end());
    std::cout << "start: is_heap? " << std::is_heap(v.begin(), v.end())
              << ", front " << v.front() << '\n';

    // ANTI-PATTERN: change the container behind the heap's back. A plain
    // push_back drops the value at the end without sifting it up, so the range is
    // no longer a heap -- and if the new value was the largest, front() is wrong.
    v.push_back(999);
    std::cout << "after raw push_back(999): is_heap? "
              << std::is_heap(v.begin(), v.end())
              << ", front " << v.front() << "   <- 999 is not at the top\n";

    // FIX: after push_back, call push_heap to restore the invariant. (The same
    // applies to writing THROUGH an element: change a value and the heap property
    // may break; you must re-heapify, e.g. std::make_heap, since there is no
    // standard "fix one changed element" operation.)
    std::push_heap(v.begin(), v.end());
    std::cout << "after push_heap:          is_heap? "
              << std::is_heap(v.begin(), v.end())
              << ", front " << v.front() << '\n';
    return 0;
}
