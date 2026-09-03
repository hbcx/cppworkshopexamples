#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main() {
    // A standard function object is a TYPE whose instances are callable. Make
    // one and call it like a function.
    std::plus<int> add;
    std::multiplies<int> mul;
    std::cout << "add(2, 3) = " << add(2, 3) << "\n";   // 5
    std::cout << "mul(4, 5) = " << mul(4, 5) << "\n";   // 20

    // They come in families, each wrapping one operator:
    //   arithmetic: plus, minus, multiplies, divides, modulus, negate
    //   comparison: equal_to, not_equal_to, less, greater, less_equal, greater_equal
    //   logical:    logical_and, logical_or, logical_not
    //   bitwise:    bit_and, bit_or, bit_xor  (bit_not since C++14)
    std::greater<int> gt;
    std::cout << std::boolalpha << "gt(7, 2) = " << gt(7, 2) << "\n";   // true

    // The point: because each is a named type with a default constructor, you can
    // pass one to any algorithm that takes a callable -- here, sort descending.
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2};
    std::sort(v.begin(), v.end(), std::greater<int>{});
    std::cout << "sorted desc:";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";   // 9 5 4 3 2 1 1
    return 0;
}
