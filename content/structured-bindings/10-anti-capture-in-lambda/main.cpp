#include <iostream>
#include <utility>

// Anti-pattern: capturing a structured binding in a lambda under C++17. A binding
// name is not an ordinary variable -- it names part of a hidden object -- and
// C++17 forbids capturing it in a lambda. (C++20 relaxed this.) The fix is to
// copy the value into a normal local first, then capture that.

std::pair<int, int> makePair() { return {10, 20}; }

int main() {
    auto [a, b] = makePair();

    // C++17: this would be ill-formed --
    //     auto f = [a, b] { return a + b; };   // error in C++17: cannot capture
    //                                           // a structured binding
    //
    // FIX: introduce ordinary locals and capture those.
    int x = a;
    int y = b;
    auto sum = [x, y] { return x + y; };
    std::cout << "sum via captured locals = " << sum() << "\n";        // 30

    // An init-capture also makes a real variable to capture:
    auto product = [p = a * b] { return p; };
    std::cout << "product via init-capture = " << product() << "\n";   // 200
    return 0;
}
