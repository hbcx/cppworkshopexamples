// Anti-pattern: top() or pop() on an empty priority_queue.
//
// Both are undefined on an empty queue -- there is no element. Check empty()
// before every access, which the standard drain loop does.

#include <iostream>
#include <queue>

int main() {
    std::priority_queue<int> pq;

    // BAD (undefined -- described, not run):
    //   int x = pq.top();   // empty: no top element
    //   pq.pop();           // empty: nothing to remove

    // GOOD: guard the access.
    if (!pq.empty()) {
        std::cout << pq.top() << '\n';
        pq.pop();
    } else {
        std::cout << "queue empty, nothing to take\n";
    }

    // GOOD: the drain loop checks empty() before each top()/pop().
    for (int x : {3, 1, 4, 1, 5}) pq.push(x);
    std::cout << "draining:";
    while (!pq.empty()) { std::cout << ' ' << pq.top(); pq.pop(); }
    std::cout << '\n';
}
