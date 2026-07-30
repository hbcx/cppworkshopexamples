#include <iostream>
#include <optional>
#include <string>

static int calls = 0;

// A "default" that is expensive to produce -- here it just counts its calls.
std::string expensiveDefault() {
    ++calls;
    return "computed-default";
}

int main() {
    std::optional<std::string> present = "real";

    // Anti-pattern: value_or's argument is an ordinary function argument, so it
    // is ALWAYS evaluated -- even when the optional has a value and the default
    // is discarded. expensiveDefault() runs here despite 'present' being set.
    calls = 0;
    std::string a = present.value_or(expensiveDefault());
    std::cout << "value_or result = " << a << '\n';
    std::cout << "expensiveDefault calls (value_or) = " << calls << '\n';  // 1 -- wasted

    // Fix: compute the default only when actually needed.
    calls = 0;
    std::string b = present ? *present : expensiveDefault();
    std::cout << "if/else result  = " << b << '\n';
    std::cout << "expensiveDefault calls (if/else)  = " << calls << '\n';  // 0

    // (C++23's or_else takes a CALLABLE, so the default runs only on the empty
    //  path -- the functional version of the same fix.)
    return 0;
}
