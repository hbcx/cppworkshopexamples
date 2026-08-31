// The range constructor heapifies in one O(n) pass. Pushing the same elements
// one at a time is O(n log n). When you already have the data, use the range
// constructor -- same heap, less work.

#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};

    // O(n): copy the range in and heapify once.
    std::priority_queue<int> fromRange(data.begin(), data.end());

    // O(n log n): the same result the slower way.
    std::priority_queue<int> fromPushes;
    for (int x : data) fromPushes.push(x);

    std::cout << "range-built top:  " << fromRange.top() << '\n';   // 9
    std::cout << "push-built top:   " << fromPushes.top() << '\n';  // 9

    std::cout << "range-built drains:";
    while (!fromRange.empty()) { std::cout << ' ' << fromRange.top(); fromRange.pop(); }
    std::cout << '\n';
}
