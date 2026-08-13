#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct Product {
    std::string name;
    int price;
};

int main() {
    // Sorted by price ascending.
    std::vector<Product> catalog{
        {"pen", 2}, {"mug", 8}, {"lamp", 20}, {"chair", 45}};

    // ranges::lower_bound takes the range, the value, a comparator ({} = default),
    // and a PROJECTION, so you search by a member: the first product priced >= 10.
    auto it = std::ranges::lower_bound(catalog, 10, {}, &Product::price);
    std::cout << "first product >= 10: " << it->name << " (" << it->price << ")\n";

    // ranges::binary_search answers presence the same way, by the projected key.
    bool has20 = std::ranges::binary_search(catalog, 20, {}, &Product::price);
    std::cout << "a product priced 20? " << std::boolalpha << has20 << '\n';
    return 0;
}
