// Anti-pattern: choosing the comparator that gives the opposite heap.
//
// priority_queue puts the GREATEST element (by the comparator) on top. So the
// default std::less -- and any a < b comparator -- is a MAX-heap. Wanting the
// smallest and writing the natural a < b gives the wrong end. Use std::greater.

#include <iostream>
#include <queue>
#include <vector>

int main() {
    // We wanted the SMALLEST on top, but the default is std::less -> a max-heap.
    std::priority_queue<int> wanted_smallest_but_wrong;
    // Correct: std::greater makes a min-heap.
    std::priority_queue<int, std::vector<int>, std::greater<int>> right;

    for (int x : {5, 2, 8, 1, 9}) { wanted_smallest_but_wrong.push(x); right.push(x); }

    std::cout << "default (std::less) top:  "
              << wanted_smallest_but_wrong.top() << "  (largest -- not what we wanted)\n";
    std::cout << "std::greater top:         "
              << right.top() << "  (smallest -- correct min-heap)\n";
}
