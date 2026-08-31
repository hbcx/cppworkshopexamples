// std::priority_queue always hands back the greatest element with top(),
// whatever order you pushed. The default is a max-heap: top() is the largest by
// the element's < operator. Interface is push / top / pop, no iteration.

#include <iostream>
#include <queue>

int main() {
    std::priority_queue<int> pq;         // max-heap by default
    for (int x : {3, 1, 4, 1, 5, 9, 2}) pq.push(x);

    std::cout << "size: " << pq.size() << ", top (largest): " << pq.top() << '\n';

    std::cout << "draining largest-first:";
    while (!pq.empty()) {
        std::cout << ' ' << pq.top();    // always the current maximum
        pq.pop();
    }
    std::cout << '\n';
}
