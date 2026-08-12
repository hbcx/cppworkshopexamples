#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Product {
    std::string sku;
    int stock;
};

int main() {
    std::vector<Product> catalog{
        {"A-100", 5},
        {"B-200", 0},
        {"C-300", 12},
    };

    // std::ranges::find (C++20) takes the whole range and, with a PROJECTION,
    // searches by a member: match the product whose sku equals "C-300".
    auto it = std::ranges::find(catalog, "C-300", &Product::sku);
    if (it != catalog.end())
        std::cout << "found " << it->sku << " with stock " << it->stock << '\n';

    // ranges::find_if with a projection: the first product that is out of stock.
    // The predicate sees the projected value (the stock int), not the whole Product.
    auto out = std::ranges::find_if(catalog, [](int s){ return s == 0; }, &Product::stock);
    if (out != catalog.end())
        std::cout << "first out of stock: " << out->sku << '\n';
    return 0;
}
