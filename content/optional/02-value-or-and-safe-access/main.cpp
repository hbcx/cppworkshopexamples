#include <iostream>
#include <optional>

int main() {
    std::optional<int> present = 7;
    std::optional<int> absent;

    // value_or returns the value if engaged, otherwise the fallback you pass.
    std::cout << "present.value_or(-1) = " << present.value_or(-1) << '\n';  // 7
    std::cout << "absent.value_or(-1)  = " << absent.value_or(-1) << '\n';   // -1

    // value() returns the value but THROWS std::bad_optional_access if empty --
    // a checked access with a defined failure, unlike bare *.
    try {
        std::cout << absent.value() << '\n';
    } catch (const std::bad_optional_access&) {
        std::cout << "value() on empty threw bad_optional_access\n";
    }

    // Dereferencing an empty optional with * or -> is UNDEFINED behaviour, not a
    // throw -- never do it without checking first:
    //     std::cout << *absent;   // UB: no value to read
    std::cout << "*present = " << *present << '\n';
    return 0;
}
