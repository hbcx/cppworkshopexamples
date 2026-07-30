#include <any>
#include <iostream>
#include <string>
#include <typeinfo>

int main() {
    std::cout << std::boolalpha;
    std::any a = 1;                      // int
    std::cout << "int?    " << (a.type() == typeid(int)) << '\n';

    // Assigning a different type replaces both the value AND the stored type --
    // unlike variant, the set of types is open.
    a = 3.5;                             // now double
    std::cout << "double? " << (a.type() == typeid(double)) << '\n';
    a = std::string("text");             // now string
    std::cout << "string? " << (a.type() == typeid(std::string)) << '\n';

    // emplace constructs a new value in place, changing the type again.
    a.emplace<char>('z');
    std::cout << "char?   " << (a.type() == typeid(char))
              << ", value = " << std::any_cast<char>(a) << '\n';

    // reset() drops the value and makes the any empty.
    a.reset();
    std::cout << "after reset, has_value? " << a.has_value() << '\n';
    return 0;
}
