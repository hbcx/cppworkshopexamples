#include <any>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

// Render an any by checking its stored type against the ones we know.
void describe(const std::any& a) {
    if (!a.has_value()) {
        std::cout << "(empty)\n";
    } else if (a.type() == typeid(int)) {
        std::cout << "int: " << std::any_cast<int>(a) << '\n';
    } else if (a.type() == typeid(double)) {
        std::cout << "double: " << std::any_cast<double>(a) << '\n';
    } else if (a.type() == typeid(std::string)) {
        std::cout << "string: " << std::any_cast<std::string>(a) << '\n';
    } else {
        std::cout << "unknown type\n";   // the fallback the compiler cannot rule out
    }
}

int main() {
    // A heterogeneous store: each slot may hold a different type. any fits when
    // the set of types is open -- not known or fixed when this code is written.
    std::vector<std::any> row;
    row.push_back(42);
    row.push_back(3.14);
    row.push_back(std::string("hello"));
    row.emplace_back();                 // an empty slot
    row.push_back('c');                 // a type describe() does not handle

    for (const std::any& cell : row) {
        describe(cell);
    }
    return 0;
}
