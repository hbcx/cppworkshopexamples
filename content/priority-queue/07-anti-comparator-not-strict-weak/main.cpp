// Anti-pattern: a comparator that is not a strict weak ordering.
//
// The comparator must return false for equal arguments (comp(a, a) == false). A
// comparator using <= reports an element as less than itself, and the heap
// operations then become undefined. Compare with a strict < (or >).

#include <iostream>
#include <queue>
#include <vector>

// BAD (undefined -- described, not used):
//   struct Bad {
//       bool operator()(int a, int b) const { return a <= b; }  // comp(a,a) is true!
//   };
//   std::priority_queue<int, std::vector<int>, Bad> broken;   // undefined behaviour

// GOOD: strict comparison -- comp(a, a) is false.
struct ByValue {
    bool operator()(int a, int b) const { return a < b; }
};

int main() {
    std::priority_queue<int, std::vector<int>, ByValue> pq;
    for (int x : {4, 4, 1, 7, 7, 2}) pq.push(x);   // duplicates are fine with strict <

    std::cout << "well-formed max-heap drains:";
    while (!pq.empty()) { std::cout << ' ' << pq.top(); pq.pop(); }
    std::cout << '\n';
}
