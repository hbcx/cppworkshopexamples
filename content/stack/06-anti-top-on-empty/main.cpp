// Anti-pattern: calling top() or pop() on an empty stack.
//
// stack does no bounds checking, so top()/pop() on an empty stack is undefined:
// there is no element. Guard every access with empty().

#include <iostream>
#include <stack>

int main() {
    std::stack<int> s;

    // BAD (undefined -- described, not run):
    //   int x = s.top();   // empty: reads a non-existent element
    //   s.pop();           // empty: removes one that is not there

    // GOOD: check empty() first.
    if (!s.empty()) {
        std::cout << s.top() << '\n';
        s.pop();
    } else {
        std::cout << "stack empty, nothing to take\n";
    }

    // The safe drain pattern: check before every access.
    s.push(10); s.push(20); s.push(30);
    std::cout << "draining:";
    while (!s.empty()) {
        std::cout << ' ' << s.top();
        s.pop();
    }
    std::cout << '\n';
}
