#include <algorithm>
#include <iostream>
#include <string>

struct Product {
    std::string name;
    int price;      // in cents
};

int main() {
    Product a{"basic", 1200};
    Product b{"pro", 900};

    // All four take an optional comparator as the LAST argument, so "smaller"
    // and "larger" can mean anything. Here: cheaper / pricier by the price field.
    auto byPrice = [](const Product& x, const Product& y) { return x.price < y.price; };

    const Product& cheaper = std::min(a, b, byPrice);
    const Product& pricier = std::max(a, b, byPrice);
    std::cout << "cheaper: " << cheaper.name << " (" << cheaper.price << ")\n";
    std::cout << "pricier: " << pricier.name << " (" << pricier.price << ")\n";

    // Pick the longest of several strings: comparator on length, list form.
    std::string longest = std::max({std::string("hi"), std::string("world"),
                                    std::string("ok")},
                                   [](const std::string& x, const std::string& y) {
                                       return x.size() < y.size();
                                   });
    std::cout << "longest word: " << longest << '\n';

    // clamp with a comparator too: hold a price into a [floor, ceiling] band,
    // comparing by the same field.
    Product floor{"floor", 1000}, ceil{"ceil", 2000};
    Product held = std::clamp(Product{"deal", 500}, floor, ceil, byPrice);
    std::cout << "clamped price: " << held.price << '\n';
    return 0;
}
