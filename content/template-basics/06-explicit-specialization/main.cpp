#include <iostream>
#include <string>

// The primary template: the generic answer for any type.
template <class T>
struct TypeName {
    static std::string get() { return "unknown"; }
};

// FULL (explicit) specialization: a separate definition for one exact type.
// TypeName<int> uses this; everything else uses the primary.
template <>
struct TypeName<int> {
    static std::string get() { return "int"; }
};
template <>
struct TypeName<bool> {
    static std::string get() { return "bool"; }
};

int main() {
    std::cout << "int:    " << TypeName<int>::get() << "\n";      // int
    std::cout << "bool:   " << TypeName<bool>::get() << "\n";     // bool
    std::cout << "double: " << TypeName<double>::get() << "\n";   // unknown (primary)
    return 0;
}
