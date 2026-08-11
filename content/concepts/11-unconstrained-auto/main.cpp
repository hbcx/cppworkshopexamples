#include <concepts>
#include <iostream>
#include <string>

// BAD: `auto` alone is an UNCONSTRAINED template parameter -- it accepts every
// type, and the check (if any) happens deep inside the body. This is exactly the
// pre-concepts error experience concepts were meant to remove.
auto doubledBad(auto x) { return x + x; }

// GOOD: `std::integral auto` is a CONSTRAINED abbreviated template. A wrong type
// is rejected at the call site with a clear message.
auto doubledGood(std::integral auto x) { return x + x; }

int main() {
    std::cout << doubledBad(21)  << "\n";     // 42 -- happens to work
    std::cout << doubledGood(21) << "\n";     // 42 -- and is constrained

    std::cout << doubledBad(std::string("ab")) << "\n";  // "abab": + concatenates,
    //   so the missing constraint stays hidden here -- a type without + would
    //   instead fail deep INSIDE the body, far from this call.
    // doubledGood(std::string("ab"));  // rejected AT THE CALL: std::string is
    //   not std::integral -- clear, early, local.
    std::cout << "done\n";
}
