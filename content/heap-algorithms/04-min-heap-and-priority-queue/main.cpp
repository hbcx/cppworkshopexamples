#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    // By default make_heap builds a MAX-heap. Pass std::greater to invert the
    // comparator and get a MIN-heap: now the SMALLEST element is at the front.
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    std::make_heap(v.begin(), v.end(), std::greater<int>());
    std::cout << "min-heap front (smallest): " << v.front() << '\n';

    // The same comparator must be passed to EVERY heap call on this range --
    // push_heap, pop_heap, sort_heap -- or the invariant is meaningless.
    v.push_back(0);
    std::push_heap(v.begin(), v.end(), std::greater<int>());
    std::cout << "after pushing 0, front: " << v.front() << '\n';

    // In practice you rarely run the raw operations by hand: std::priority_queue
    // wraps make_heap/push_heap/pop_heap over a container and enforces the
    // comparator for you. Default is a max-heap (top() is the largest).
    std::priority_queue<int> pq;
    for (int x : {3, 1, 4, 1, 5}) pq.push(x);
    std::cout << "priority_queue drains largest-first:";
    while (!pq.empty()) { std::cout << ' ' << pq.top(); pq.pop(); }
    std::cout << '\n';

    // Use the raw algorithms when you need the heap over storage you also access
    // directly; use priority_queue when you just want the queue behaviour.
    return 0;
}
