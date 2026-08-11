#include <iostream>
#include <type_traits>
#include <vector>

enum Color { Red, Green };

// A generic describe() that branches on the type CATEGORY at compile time.
template <class T>
void describe(const char* label) {
    std::cout << label << ": ";
    if constexpr (std::is_integral_v<T>)            std::cout << "integer\n";
    else if constexpr (std::is_floating_point_v<T>) std::cout << "floating point\n";
    else if constexpr (std::is_pointer_v<T>)        std::cout << "pointer\n";
    else if constexpr (std::is_enum_v<T>)           std::cout << "enum\n";
    else if constexpr (std::is_class_v<T>)          std::cout << "class\n";
    else                                            std::cout << "something else\n";
}

int main() {
    describe<int>("int");
    describe<double>("double");
    describe<char*>("char*");
    describe<Color>("Color");
    describe<std::vector<int>>("vector");

    // The categories are consistent and exact about qualifiers.
    static_assert(std::is_arithmetic_v<int> && std::is_arithmetic_v<double>);
    static_assert(std::is_array_v<int[4]> && !std::is_array_v<int>);
    static_assert(std::is_reference_v<int&> && std::is_reference_v<int&&>);
    static_assert(!std::is_integral_v<int&>);   // a reference is not integral
    return 0;
}
