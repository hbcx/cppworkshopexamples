// stack::pop returns void. You read the top with top(), then remove it with
// pop(). The split is on purpose: a pop that returned the value by copy could
// throw AFTER removing the element, losing it with no way to recover.

#include <iostream>
#include <stack>
#include <string>
#include <utility>

int main() {
    std::stack<std::string> s;
    s.push("first");
    s.push("second");

    // The idiom: read, then remove.
    std::string value = s.top();   // read (safe to retry if it threw)
    s.pop();                       // remove (does not throw)
    std::cout << "took: " << value << '\n';

    // When a copy is expensive, move out of top() before popping.
    std::string moved = std::move(s.top());
    s.pop();
    std::cout << "took (moved): " << moved << '\n';

    std::cout << "remaining: " << s.size() << '\n';
}
