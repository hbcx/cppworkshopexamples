#include <iostream>
#include <string>
#include <tuple>

int main() {
    // A tuple holds a fixed number of values of possibly different types --
    // pair generalized from two elements to any number.
    std::tuple<int, std::string, double> row(1, "widget", 3.5);

    // There are no .first/.second names. Access is by compile-time index with
    // std::get<I>; the index must be a constant known at compile time.
    std::cout << std::get<0>(row) << ' '
              << std::get<1>(row) << ' '
              << std::get<2>(row) << '\n';

    // std::get returns a reference, so you can assign through it.
    std::get<2>(row) = 4.0;
    std::cout << "updated price = " << std::get<2>(row) << '\n';

    // make_tuple deduces the element types (and decays like make_pair).
    auto t = std::make_tuple(42, 'x', std::string("hi"));
    std::cout << std::get<0>(t) << ' ' << std::get<1>(t) << ' '
              << std::get<2>(t) << '\n';

    // An out-of-range index is a COMPILE error, not a runtime one:
    //     std::get<3>(row);   // does not compile -- only 0..2 exist
    return 0;
}
