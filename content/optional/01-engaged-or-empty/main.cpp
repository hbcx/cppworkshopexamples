#include <iostream>
#include <optional>
#include <string>

int main() {
    // An optional<T> either holds a T (engaged) or holds nothing (disengaged).
    std::optional<int> present = 42;
    std::optional<int> absent;                     // default: empty
    std::optional<int> alsoAbsent = std::nullopt;  // explicit empty

    std::cout << std::boolalpha;
    // has_value() -- or the contextual bool conversion -- asks what state it is in.
    std::cout << "present.has_value() = " << present.has_value() << '\n';
    std::cout << "absent (as bool)    = " << static_cast<bool>(absent) << '\n';

    // When engaged, read the value with * or -> (only when engaged -- see 02).
    std::cout << "*present = " << *present << '\n';

    std::optional<std::string> name = "Ada";
    std::cout << "length via -> = " << name->size() << '\n';

    // The bool conversion makes if-guards read naturally.
    if (present) std::cout << "present holds " << *present << '\n';
    if (!absent) std::cout << "absent holds nothing\n";
    (void) alsoAbsent;
    return 0;
}
