#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

static const std::vector<int> input{40, 10, 30, 20};

int main() {
    // FIFO: the OLDEST element comes back. Accessor: front().
    std::queue<int> fifo;
    for (int x : input) {
        fifo.push(x);
    }
    std::cout << "queue          (oldest first):";
    while (!fifo.empty()) {
        std::cout << ' ' << fifo.front();
        fifo.pop();
    }
    std::cout << '\n';

    // LIFO: the NEWEST element comes back. Accessor: top(), not front().
    std::stack<int> lifo;
    for (int x : input) {
        lifo.push(x);
    }
    std::cout << "stack          (newest first):";
    while (!lifo.empty()) {
        std::cout << ' ' << lifo.top();
        lifo.pop();
    }
    std::cout << '\n';

    // Best-first: the LARGEST element comes back -- std::less gives a MAX-heap.
    std::priority_queue<int> maxHeap;
    for (int x : input) {
        maxHeap.push(x);
    }
    std::cout << "priority_queue (largest first):";
    while (!maxHeap.empty()) {
        std::cout << ' ' << maxHeap.top();
        maxHeap.pop();
    }
    std::cout << '\n';

    // Flip the comparator to std::greater for the min-heap people usually want.
    // The container parameter must be spelled out to reach the third one.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    for (int x : input) {
        minHeap.push(x);
    }
    std::cout << "priority_queue (smallest first):";
    while (!minHeap.empty()) {
        std::cout << ' ' << minHeap.top();
        minHeap.pop();
    }
    std::cout << '\n';
    return 0;
}
