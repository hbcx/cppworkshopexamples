// std::move does NOT move anything. It is a cast: it turns its argument into an
// xvalue (a kind of rvalue), so overload resolution picks the T&& overload -- the
// move constructor or move assignment. The actual stealing happens in that
// overload, not in std::move. So std::move(x) with nothing consuming it leaves x
// untouched; only handing the xvalue to a move-taking function transfers anything.
#include <iostream>
#include <string>
#include <utility>

int main() {
    std::string a = "hello world";

    // A cast on its own: nothing consumes the result, so a is unchanged.
    (void)std::move(a);
    std::cout << "after a bare std::move, a = \"" << a << "\"\n";   // still "hello world"

    // Handing the xvalue to a move constructor is what transfers the buffer.
    std::string b = std::move(a);      // selects string's move constructor
    std::cout << "b = \"" << b << "\"\n";

    // a is now valid but UNSPECIFIED: assign to it or destroy it, do not read it.
    std::cout << "b took the content; a must be reassigned before it is read again\n";
    return 0;
}
