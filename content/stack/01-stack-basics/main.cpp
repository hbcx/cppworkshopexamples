// std::stack is LIFO: push adds to the top, top reads it, pop removes it. It is
// an adaptor exposing only that narrow interface -- no iteration, no indexing --
// which is what keeps the last-in-first-out discipline safe.

#include <iostream>
#include <stack>

int main() {
    std::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);              // top is now 3

    std::cout << "size: " << s.size() << ", top: " << s.top() << '\n';

    std::cout << "popping (LIFO):";
    while (!s.empty()) {
        std::cout << ' ' << s.top();   // read the top
        s.pop();                       // then remove it
    }
    std::cout << '\n';
    std::cout << "empty now? " << std::boolalpha << s.empty() << '\n';
}
