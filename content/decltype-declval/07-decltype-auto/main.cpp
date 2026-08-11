#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// `auto` deduction STRIPS references and const: it always gives a value type.
// `decltype(auto)` deduces EXACTLY what the return expression is, keeping the
// reference -- so this accessor returns std::string&, not a copy.
struct Registry {
    std::vector<std::string> items{"a", "b", "c"};
    decltype(auto) at(std::size_t i) { return items[i]; }   // items[i] is std::string&
};

int main() {
    Registry r;

    // auto copies: modifying the copy leaves the original untouched.
    auto copy = r.at(0);
    copy = "CHANGED";
    std::cout << "after auto copy:         items[0]=" << r.at(0) << "\n";   // still "a"

    // decltype(auto) follows at()'s real return type -- a reference.
    decltype(auto) ref = r.at(1);
    ref = "CHANGED";
    std::cout << "after decltype(auto) ref: items[1]=" << r.at(1) << "\n";  // "CHANGED"
    return 0;
}
