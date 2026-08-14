#include <algorithm>
#include <iostream>

int main() {
    int a = 7, b = 3;

    // std::min / std::max take VALUES (arguments), not a range. They return the
    // smaller / larger of the two. This is the everyday "pick one of two".
    std::cout << "min(7, 3) = " << std::min(a, b) << '\n';
    std::cout << "max(7, 3) = " << std::max(a, b) << '\n';

    // For more than two values, pass an initializer_list in braces -- one call
    // finds the smallest or largest of them all, no nesting of min(min(...)).
    std::cout << "min of {4,8,1,6,2} = " << std::min({4, 8, 1, 6, 2}) << '\n';
    std::cout << "max of {4,8,1,6,2} = " << std::max({4, 8, 1, 6, 2}) << '\n';

    // A ternary does the same for two values, but min/max read clearer, compose
    // into the list form, and take a comparator when the order is not the
    // built-in < (see the custom-comparator example).
    return 0;
}
