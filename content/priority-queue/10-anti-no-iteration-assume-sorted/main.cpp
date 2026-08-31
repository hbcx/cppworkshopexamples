// Anti-pattern: iterating a priority_queue or assuming it is sorted.
//
// There are no iterators, and the elements are in heap order, not sorted order.
// Only top() is the extreme. To read them all in order you must drain, which
// empties the queue -- so copy it first if you still need it.

#include <iostream>
#include <queue>

int main() {
    std::priority_queue<int> pq;
    for (int x : {3, 1, 4, 1, 5, 9, 2}) pq.push(x);

    // BAD (does not compile -- described):
    //   for (int x : pq) ...            // no begin()/end()
    //   int second = pq /* [1] */;      // no way to peek past the top

    // GOOD: drain a COPY so the original queue survives.
    std::priority_queue<int> copy = pq;
    std::cout << "all elements in priority order:";
    while (!copy.empty()) { std::cout << ' ' << copy.top(); copy.pop(); }
    std::cout << '\n';

    std::cout << "original still has " << pq.size() << " elements, top " << pq.top() << '\n';
}
