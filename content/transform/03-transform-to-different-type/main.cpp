#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

struct Product {
    std::string name;
    double price;
};

int main() {
    // The output type does not have to match the input. Turn status codes into
    // their text labels: input int, output std::string.
    std::vector<int> codes{200, 404, 500};
    std::vector<std::string> labels;
    std::transform(codes.begin(), codes.end(), std::back_inserter(labels),
        [](int c){ return "HTTP " + std::to_string(c); });
    for (const auto& s : labels) std::cout << s << '\n';

    // A common shape: pull one field out of each record into its own list.
    std::vector<Product> cart{
        {"pen", 1.5}, {"mug", 8.0}, {"book", 12.5},
    };
    std::vector<std::string> names;
    std::transform(cart.begin(), cart.end(), std::back_inserter(names),
        [](const Product& p){ return p.name; });
    std::cout << "names: ";
    for (const auto& n : names) std::cout << n << ' ';
    std::cout << '\n';
    return 0;
}
