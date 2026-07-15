#include <iostream>
#include <set>
#include <string>

static void print(const std::set<std::string>& s, const char* label) {
    std::cout << label;
    for (const auto& x : s) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    std::set<std::string> names{"bob", "carol", "dave"};
    print(names, "before:                ");

    // A set element is const: *it has type const std::string&, because moving it
    // out of order would break the tree. So this would NOT compile:
    //   *names.find("bob") = "alice";   // error: assignment of read-only location

    // To "modify" an element, erase the old value and insert the new one.
    names.erase("bob");
    names.insert("alice");
    print(names, "after rename bob->alice:");

    // (C++17 extract() does the same without freeing and reallocating the node.)
    return 0;
}
