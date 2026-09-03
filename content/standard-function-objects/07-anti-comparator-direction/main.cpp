#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    // ANTI-PATTERN: guessing a comparator's direction from its name. The rule is
    // one sentence: comp(a, b) == true means "a comes before b in the ordering".
    // Read every use through that rule, not through the word greater/less.

    std::vector<int> v{3, 1, 4, 1, 5};

    // std::sort with std::greater: "a before b when a > b" -> largest first.
    std::sort(v.begin(), v.end(), std::greater<int>{});
    std::cout << "sort + greater:";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";   // 5 4 3 1 1  (descending)

    // std::priority_queue reads the comparator from the OTHER end: it pops the
    // element that comes LAST in that order. So std::greater (which orders
    // larger-first) makes the SMALLEST pop first -- a MIN-heap. The classic trap.
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int x : {3, 1, 4, 1, 5}) pq.push(x);
    std::cout << "pq + greater pops:";
    while (!pq.empty()) { std::cout << " " << pq.top(); pq.pop(); }
    std::cout << "\n";   // 1 1 3 4 5  (ascending -> min-heap)

    // Same functor, opposite-looking result -- because sort and priority_queue
    // read the comparator from opposite ends. See the priority-queue chapter.
    return 0;
}
