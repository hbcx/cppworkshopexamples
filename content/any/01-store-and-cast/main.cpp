#include <any>
#include <iostream>
#include <string>
#include <typeinfo>

int main() {
    // std::any holds a single value of ANY type, chosen at assignment.
    std::any a = 42;                 // holds an int
    std::cout << std::boolalpha;
    std::cout << "has_value? " << a.has_value() << '\n';

    // Recover the value with any_cast<T>, where T must be the stored type.
    std::cout << "as int = " << std::any_cast<int>(a) << '\n';

    // The SAME any can later hold a different type (see 03).
    a = std::string("hello");
    std::cout << "as string = " << std::any_cast<std::string>(a) << '\n';

    // type() reports the stored type; compare it against typeid(T) rather than
    // printing type().name(), which is an implementation-defined string.
    std::cout << "holds string? " << (a.type() == typeid(std::string)) << '\n';

    std::any empty;                  // default: holds nothing
    std::cout << "empty.has_value()? " << empty.has_value() << '\n';
    return 0;
}
