#include <iostream>
#include <string>
#include <variant>

int main() {
    std::variant<int, std::string> v = 7;
    std::cout << std::boolalpha;

    // get<T> returns the value if T is active, else THROWS bad_variant_access.
    std::cout << "get<int> = " << std::get<int>(v) << '\n';
    try {
        (void) std::get<std::string>(v);   // int is active -> throws
    } catch (const std::bad_variant_access&) {
        std::cout << "get<std::string> threw: int is the active alternative\n";
    }

    // get_if takes a pointer and returns a pointer to the value, or nullptr when
    // that alternative is not active -- the checked, no-throw access.
    if (int* p = std::get_if<int>(&v)) {
        std::cout << "get_if<int> ok: " << *p << '\n';
    }
    if (std::get_if<std::string>(&v) == nullptr) {
        std::cout << "get_if<std::string>: nullptr (not active)\n";
    }

    // holds_alternative is the boolean form of the same question.
    std::cout << "holds int? " << std::holds_alternative<int>(v) << '\n';
    return 0;
}
