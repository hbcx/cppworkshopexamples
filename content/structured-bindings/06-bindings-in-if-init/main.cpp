#include <iostream>
#include <map>
#include <string>

// C++17 lets an if or switch carry an initializer, and a structured binding is
// the perfect fit: unpack a function's result AND test part of it in one place,
// keeping the names scoped to the if.

int main() {
    std::map<std::string, int> inventory{{"apple", 3}};

    // map::insert returns { iterator, bool }. Bind and test in one statement.
    if (auto [it, inserted] = inventory.insert({"banana", 5}); inserted) {
        std::cout << "added " << it->first << " = " << it->second << "\n";
    } else {
        std::cout << it->first << " was already present\n";
    }

    // Try the same key again: this time inserted is false, and it points at the
    // existing element.
    if (auto [it, inserted] = inventory.insert({"banana", 99}); inserted) {
        std::cout << "added " << it->first << "\n";
    } else {
        std::cout << it->first << " already present, keeping " << it->second << "\n";
    }

    // The binding names live only inside the if/else -- nothing leaks into the
    // surrounding scope.
    return 0;
}
