#include <any>
#include <iostream>
#include <string>
#include <typeinfo>

int main() {
    std::cout << std::boolalpha;

    // Anti-pattern: casting to a type that is NOT exactly what was stored.

    // Trap 1: a string literal decays to const char*, NOT std::string. So this
    // any holds a const char*, and any_cast<std::string> throws.
    std::any a = "hi";
    std::cout << "holds const char*? " << (a.type() == typeid(const char*)) << '\n';
    std::cout << "holds std::string? " << (a.type() == typeid(std::string)) << '\n';
    try {
        (void) std::any_cast<std::string>(a);
    } catch (const std::bad_any_cast&) {
        std::cout << "any_cast<std::string> threw: stored type is const char*\n";
    }

    // Trap 2: no numeric conversions. An int does not come back as a long.
    std::any n = 42;
    try {
        (void) std::any_cast<long>(n);
    } catch (const std::bad_any_cast&) {
        std::cout << "any_cast<long> threw: stored type is int, not long\n";
    }

    // Fix: store and cast the SAME (decayed) type -- wrap literals in std::string
    // and cast to exactly what you put in.
    std::any s = std::string("hi");
    std::cout << "fixed: " << std::any_cast<std::string>(s) << '\n';
    return 0;
}
