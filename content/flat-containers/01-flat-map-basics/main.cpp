// std::flat_map (C++23) is a sorted key-to-value container stored in a vector.
// It reads like std::map -- at, find, contains, ordered iteration -- but keeps
// the elements packed. Iterate with `auto [k, v]` (a proxy), not `auto&`.

#include <flat_map>
#include <iostream>
#include <string>

int main() {
    std::flat_map<std::string, int> stock;
    stock["banana"] = 3;
    stock["apple"] = 5;
    stock["cherry"] = 1;

    std::cout << "in key order:\n";
    for (auto [name, count] : stock)          // auto, not auto& (proxy reference)
        std::cout << "  " << name << " -> " << count << '\n';

    std::cout << "at(\"apple\") = " << stock.at("apple") << '\n';
    std::cout << "contains(\"cherry\")? " << std::boolalpha
              << stock.contains("cherry") << '\n';
    std::cout << "find(\"kiwi\") == end()? " << (stock.find("kiwi") == stock.end()) << '\n';
    std::cout << "size = " << stock.size() << '\n';
}
