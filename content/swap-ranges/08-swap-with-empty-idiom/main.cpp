#include <vector>
#include <iostream>

int main() {
    std::vector<int> v(1000, 7);
    v.clear();

    // After clear(), the size is 0 but the CAPACITY (the allocated buffer) is kept.
    std::cout << std::boolalpha;
    std::cout << "after clear: size=" << v.size()
              << ", capacity still >= 1000? " << (v.capacity() >= 1000) << '\n';

    // BAD idea: expecting clear() (or assigning an empty vector) to free the memory.
    // It does not -- the capacity stays, holding the allocation for reuse.

    // GOOD: the swap-with-empty idiom releases it. Swapping v with a fresh empty
    // vector gives v the empty one's (zero) capacity, and the unnamed temporary
    // takes the old buffer and frees it at the end of the full expression.
    std::vector<int>().swap(v);
    std::cout << "after swap-with-empty: size=" << v.size()
              << ", capacity=" << v.capacity() << '\n';

    // (C++11 also has shrink_to_fit(), but it is a non-binding request; the swap
    // trick is the guaranteed way to drop the capacity.)
    return 0;
}
