#include <iostream>

// GOOD: return the BARE name. decltype(x) is the declared type (int), so the
// function returns by VALUE and the caller gets a safe copy.
decltype(auto) makeValue() {
    int x = 42;
    return x;
}

// BAD (described, not compiled, because it is undefined behaviour): the extra
// parentheses turn x into an lvalue EXPRESSION, so decltype((x)) is int&, and the
// function returns a reference to a local that dies at the return.
//   decltype(auto) makeDangling() {
//       int x = 42;
//       return (x);       // returns int& to a local -> DANGLING reference (UB)
//   }

int main() {
    auto v = makeValue();
    std::cout << "makeValue() = " << v << "  <- returned by value, safe\n";

    // The trap in one line: decltype(name) is the declared type, but
    // decltype((name)) -- with parentheses -- is an lvalue reference. As a
    // decltype(auto) return type, return (x); silently returns a reference to the
    // local x, destroyed as the function returns.
    std::cout << "done\n";
    return 0;
}
