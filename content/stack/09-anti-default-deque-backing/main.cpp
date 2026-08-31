// Anti-pattern: accepting the default deque backing for a performance-sensitive
// stack.
//
// A stack only works one end, so deque's never-invalidate guarantee buys nothing
// while its chunked storage costs cache locality. A vector backing keeps the
// elements contiguous. Same interface and result; only the layout changes.

#include <deque>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>

template <class Stack>
static long sumByDraining(Stack s) {   // by value: we drain a copy
    long total = 0;
    while (!s.empty()) { total += s.top(); s.pop(); }
    return total;
}

int main() {
    std::stack<int> defaultStack;                     // deque-backed (default)
    std::stack<int, std::vector<int>> hotStack;       // vector-backed (better here)

    for (int i = 1; i <= 100; ++i) { defaultStack.push(i); hotStack.push(i); }

    std::cout << "deque-backed sum:  " << sumByDraining(defaultStack) << '\n';
    std::cout << "vector-backed sum: " << sumByDraining(hotStack) << '\n';
    std::cout << "same result; the vector backing is contiguous and cache-friendly\n";
}
