#include <limits>
#include <iostream>

int main() {
    // For INTEGER types, min() is the most negative value; nothing is below it,
    // so min() and lowest() are the same.
    std::cout << "int:    min=" << std::numeric_limits<int>::min()
              << " max=" << std::numeric_limits<int>::max()
              << " lowest=" << std::numeric_limits<int>::lowest() << "\n";

    // For FLOATING types, min() is the smallest POSITIVE normal value -- NOT the
    // most negative. The most negative finite value is lowest() == -max().
    std::cout << "double: min=" << std::numeric_limits<double>::min()
              << " max=" << std::numeric_limits<double>::max()
              << " lowest=" << std::numeric_limits<double>::lowest() << "\n";

    std::cout << "float:  min=" << std::numeric_limits<float>::min()
              << " lowest=" << std::numeric_limits<float>::lowest() << "\n";

    // lowest() gives one name for "most negative finite" across integer AND
    // floating types -- the right seed for a running maximum in generic code.
}
