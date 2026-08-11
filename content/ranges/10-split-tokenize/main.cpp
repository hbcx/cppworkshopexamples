#include <iostream>
#include <ranges>
#include <string>

int main() {
    std::string text = "alpha,beta,gamma,delta";

    // views::split breaks a range on a delimiter, yielding a range of SUBRANGES
    // (each a view into the original, no copy). Build a std::string from a part
    // when you need an owned token.
    std::cout << "parts:\n";
    for (auto part : text | std::views::split(',')) {
        std::string token(part.begin(), part.end());
        std::cout << "  [" << token << "]\n";
    }
    return 0;
}
