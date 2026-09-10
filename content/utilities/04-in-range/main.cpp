#include <cstdint>
#include <iostream>
#include <utility>

int main() {
    long small = 42;
    long big = 100000;
    long negative = -5;

    std::cout << std::boolalpha;

    // std::in_range<T>(v) asks: is v representable in type T, with no lossy
    // narrowing? It handles signedness and width correctly.
    std::cout << "in_range<std::uint8_t>(42)     = " << std::in_range<std::uint8_t>(small) << "\n";
    std::cout << "in_range<std::uint8_t>(100000) = " << std::in_range<std::uint8_t>(big) << "\n";
    std::cout << "in_range<std::uint8_t>(-5)     = " << std::in_range<std::uint8_t>(negative) << "\n";
    std::cout << "in_range<int>(100000)          = " << std::in_range<int>(big) << "\n";
    return 0;
}
