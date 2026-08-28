#include <iostream>
#include <cstdint>

enum class Color : int { Red = 1, Green = 2, Blue = 4 };

int main() {
    // Numeric: make the truncation explicit instead of relying on a silent one.
    double pi = 3.9;
    int n = static_cast<int>(pi);
    std::cout << "double -> int: " << n << "\n";                 // 3

    // Enum to its underlying integer (scoped enums do not convert on their own):
    Color c = Color::Blue;
    int code = static_cast<int>(c);
    std::cout << "enum -> int: " << code << "\n";                // 4

    // void* back to the real type (e.g. from a C API):
    int value = 42;
    void* raw = &value;
    int* typed = static_cast<int*>(raw);
    std::cout << "void* -> int*: " << *typed << "\n";            // 42

    // Integer to a wider integer, made explicit:
    std::int8_t small = 100;
    auto wide = static_cast<std::int64_t>(small);
    std::cout << "int8 -> int64: " << wide << "\n";              // 100
    return 0;
}
