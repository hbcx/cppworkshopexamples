// Keep the k largest values of a stream with a MIN-heap of size k: its top is
// the smallest one kept, so it is exactly what to drop when a bigger value
// arrives. Memory stays O(k) however long the stream is.

#include <iostream>
#include <queue>
#include <vector>

int main() {
    const std::size_t k = 3;
    std::priority_queue<int, std::vector<int>, std::greater<int>> kLargest;  // min-heap

    for (int value : {7, 2, 9, 4, 1, 8, 5, 3, 6}) {
        kLargest.push(value);
        if (kLargest.size() > k) kLargest.pop();   // drop the smallest kept
    }

    std::cout << "k-th largest (heap top): " << kLargest.top() << '\n';   // 7

    std::cout << "the " << k << " largest (ascending):";
    while (!kLargest.empty()) { std::cout << ' ' << kLargest.top(); kLargest.pop(); }
    std::cout << '\n';   // 7 8 9
}
