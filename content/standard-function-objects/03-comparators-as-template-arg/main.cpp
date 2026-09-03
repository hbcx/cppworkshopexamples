#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

int main() {
    // The comparison objects shine as the Compare TYPE argument of an ordered
    // container: named, stateless, default-constructible, so you name the type
    // and the container builds the comparator itself.

    // A set ordered high-to-low: std::greater<int> instead of the default less.
    std::set<int, std::greater<int>> desc{3, 1, 4, 1, 5};
    std::cout << "set desc:";
    for (int x : desc) std::cout << " " << x;
    std::cout << "\n";   // 5 4 3 1

    // A map whose keys run high-to-low.
    std::map<int, std::string, std::greater<int>> ranking{{1, "a"}, {3, "c"}, {2, "b"}};
    std::cout << "map desc keys:";
    for (const auto& kv : ranking) std::cout << " " << kv.first;
    std::cout << "\n";   // 3 2 1

    // A priority_queue's third template argument is the comparator. Note the
    // inversion: std::greater makes a MIN-heap (smallest on top) -- see the
    // priority-queue chapter, and the comparator-direction anti-pattern here.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    for (int x : {5, 1, 4, 2}) minHeap.push(x);
    std::cout << "min-heap pops:";
    while (!minHeap.empty()) { std::cout << " " << minHeap.top(); minHeap.pop(); }
    std::cout << "\n";   // 1 2 4 5
    return 0;
}
