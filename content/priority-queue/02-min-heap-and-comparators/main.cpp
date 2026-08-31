// For a min-heap, pass std::greater as the comparator. It is the THIRD template
// argument, so you must also name the container (std::vector) to reach it:
//   priority_queue<int, std::vector<int>, std::greater<int>>

#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::priority_queue<int> maxHeap;                                  // default
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int x : {3, 1, 4, 1, 5}) { maxHeap.push(x); minHeap.push(x); }

    std::cout << "max-heap top (largest):  " << maxHeap.top() << '\n';  // 5
    std::cout << "min-heap top (smallest): " << minHeap.top() << '\n';  // 1

    std::cout << "min-heap drains ascending:";
    while (!minHeap.empty()) { std::cout << ' ' << minHeap.top(); minHeap.pop(); }
    std::cout << '\n';
}
