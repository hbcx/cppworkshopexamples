// Anti-pattern: keeping a reference from top() and using it after pop().
//
// top() returns a reference INTO the stack; pop() destroys that element. A
// reference held across the pop dangles. Copy (or move) the value out first.

#include <iostream>
#include <stack>
#include <string>

int main() {
    std::stack<std::string> s;
    s.push("payload");

    // BAD (dangling -- described, not run):
    //   const std::string& r = s.top();   // reference into the stack
    //   s.pop();                          // destroys that element
    //   std::cout << r;                   // r now dangles -- undefined

    // GOOD: copy the value out of top(), then pop. The local is yours.
    std::string value = s.top();   // copy
    s.pop();                       // safe: value is independent of the stack
    std::cout << "took: " << value << '\n';

    std::cout << "stack now holds " << s.size() << " elements\n";
}
