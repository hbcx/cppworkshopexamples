#include <iostream>
#include <map>
#include <string>

int main() {
    // std::map keeps its entries sorted by key at all times. We insert in a
    // jumbled order; iteration comes back sorted.
    std::map<std::string, int> ages;
    ages["Charlie"] = 30;
    ages["Alice"] = 25;
    ages["Bob"] = 28;
    ages.insert({"Dave", 40});

    std::cout << "iterating (sorted by key):\n";
    for (const auto& [name, age] : ages) {   // structured bindings (C++17)
        std::cout << "  " << name << " = " << age << '\n';
    }
    std::cout << "size = " << ages.size() << '\n';
    return 0;
}
