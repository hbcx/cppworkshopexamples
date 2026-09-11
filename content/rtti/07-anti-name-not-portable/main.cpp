#include <iostream>
#include <typeindex>
#include <typeinfo>

struct Widget {};

int main() {
    // ANTI-PATTERN: using type_info::name() for logic or persistence. Its value is
    // implementation-defined -- gcc and clang give a mangled string (below), MSVC
    // gives "struct Widget" -- and nothing guarantees stability across versions.
    std::cout << "name() (implementation-defined): " << typeid(Widget).name() << "\n";
    //
    // A string compare of name() is the WRONG way to test type identity:
    //     if (std::string(typeid(a).name()) == typeid(b).name()) ...   // fragile

    // FIX: compare type_info objects directly, or key on std::type_index.
    std::cout << std::boolalpha;
    std::cout << "same type via ==?         " << (typeid(Widget) == typeid(Widget)) << "\n";
    std::cout << "same type via type_index? "
              << (std::type_index(typeid(Widget)) == std::type_index(typeid(Widget))) << "\n";
    return 0;
}
