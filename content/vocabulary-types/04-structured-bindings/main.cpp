#include <iostream>
#include <map>
#include <string>
#include <utility>

std::pair<int, int> divmod(int a, int b) {
    return {a / b, a % b};
}

int main() {
    // Structured bindings (C++17) unpack a pair into two named variables --
    // far more readable than qr.first / qr.second.
    auto [q, r] = divmod(17, 5);
    std::cout << "q = " << q << ", r = " << r << '\n';

    // The classic use: naming the key and value while iterating a map, whose
    // element type is pair<const Key, Value>.
    std::map<std::string, int> ages{{"ada", 36}, {"bea", 41}};
    for (const auto& [name, age] : ages) {
        std::cout << name << " is " << age << '\n';
    }

    // Bind by reference to modify the mapped values in place.
    for (auto& [name, age] : ages) {
        (void)name;
        age += 1;
    }
    std::cout << "ada now " << ages["ada"] << '\n';
    return 0;
}
