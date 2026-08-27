#include <iostream>
#include <tuple>
#include <string>

// Anti-pattern: expecting to bind only some elements, or to skip one. A
// structured binding must introduce EXACTLY as many names as the object has
// elements -- no fewer, no more. There is no "skip this one" in the binding, and
// std::ignore (which works with std::tie) is NOT accepted as a binding name.

std::tuple<std::string, int, double> record() {
    return {"widget", 7, 9.99};
}

int main() {
    // Correct: one name per element, all three.
    auto [name, qty, price] = record();
    std::cout << name << " x" << qty << " @ " << price << "\n";

    // These would NOT compile (shown as comments):
    //     auto [name2, qty2] = record();               // error: 2 names, 3 elements
    //     auto [a, b, c, d]  = record();               // error: 4 names, 3 elements
    //     auto [n, std::ignore, p] = record();         // error: ignore is not a name
    //
    // If you truly want to discard elements, use std::tie with std::ignore
    // instead of a structured binding:
    std::string name4;
    double      price4;
    std::tie(name4, std::ignore, price4) = record();    // the qty is discarded
    std::cout << "via tie (qty ignored): " << name4 << " @ " << price4 << "\n";
    return 0;
}
