#include <iostream>
#include <typeinfo>

int main() {
    int n = 42;
    double d = 3.14;

    // typeid yields a const std::type_info&. name() is implementation-defined --
    // a mangled string on gcc/clang, "int" on MSVC -- so it is for debugging only.
    std::cout << "typeid(n).name()      = " << typeid(n).name() << "\n";
    std::cout << "typeid(double).name() = " << typeid(double).name() << "\n";

    // The reliable operation is comparison: are two types the same?
    std::cout << std::boolalpha;
    std::cout << "typeid(int) == typeid(n)? " << (typeid(int) == typeid(n)) << "\n";
    std::cout << "typeid(int) == typeid(d)? " << (typeid(int) == typeid(d)) << "\n";
    return 0;
}
