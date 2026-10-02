#include <iostream>
#include <string>
#include <variant>

// std::variant is the tagged union done for you: it tracks the active type, runs
// the right constructors and destructors, and gives std::visit and std::get
// instead of a hand-written tag and switch.
int main() {
    std::variant<int, std::string> v = 42;

    std::cout << "index = " << v.index() << '\n';               // 0 (int is alternative 0)
    std::cout << "get<int> = " << std::get<int>(v) << '\n';     // 42

    v = std::string("hello");                                   // switches the active type
    std::cout << "index = " << v.index() << '\n';               // 1

    // std::visit calls the visitor with whatever is active -- no manual tag.
    std::visit([](const auto& x) { std::cout << "visit: " << x << '\n'; }, v);   // visit: hello

    // Safe typed access: get_if returns nullptr when the type is not active.
    if (const std::string* s = std::get_if<std::string>(&v))
        std::cout << "holds a string of length " << s->size() << '\n';           // 5

    // std::get on the wrong type throws instead of reading unrelated bytes.
    try {
        std::get<int>(v);   // v holds a string now
    } catch (const std::bad_variant_access&) {
        std::cout << "get<int> threw bad_variant_access, as it should\n";
    }
    return 0;
}
