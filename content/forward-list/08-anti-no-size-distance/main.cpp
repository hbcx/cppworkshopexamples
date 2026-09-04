#include <forward_list>
#include <iostream>

// Count the node visits so the cost is visible without a timer: this is what
// std::distance does under the hood on a forward_list -- one step per node.
long g_steps = 0;

template <typename It>
long counted_distance(It first, It last) {
    long n = 0;
    for (; first != last; ++first) { ++n; ++g_steps; }
    return n;
}

int main() {
    std::forward_list<int> fl{1, 2, 3, 4, 5, 6, 7, 8};   // 8 nodes

    // ANTI-PATTERN: forward_list has no size(), so this asks for the length
    // every iteration. Each call walks the whole chain -> O(n) inside an O(n)
    // loop = O(n^2).
    g_steps = 0;
    for (int i = 0; i < 8; ++i) {
        long len = counted_distance(fl.begin(), fl.end());
        (void)len;   // pretend we use it
    }
    std::cout << "recomputed-length loop steps = " << g_steps << "\n";

    // FIX: compute the length ONCE and reuse it. If you need the count often,
    // that itself is the signal to use std::list (O(1) size) or std::vector.
    g_steps = 0;
    long len = counted_distance(fl.begin(), fl.end());
    for (long i = 0; i < len; ++i) {
        // work with the cached length
    }
    std::cout << "cached-length steps          = " << g_steps << "\n";
    return 0;
}
