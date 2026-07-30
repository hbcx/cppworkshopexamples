#include <iostream>
#include <string>
#include <variant>

int main() {
    // A variant holds exactly ONE value from its fixed list of alternatives.
    std::variant<int, double, std::string> v = 42;   // currently the int

    std::cout << std::boolalpha;
    // index() is the position of the active alternative; holds_alternative asks
    // about a specific type.
    std::cout << "index = " << v.index() << '\n';                        // 0
    std::cout << "holds int? " << std::holds_alternative<int>(v) << '\n';  // true
    std::cout << "value = " << std::get<int>(v) << '\n';                 // by type

    // Assigning a value of another alternative switches which one is active and
    // destroys the previous value.
    v = std::string("hello");
    std::cout << "index = " << v.index() << '\n';                        // 2
    std::cout << "value = " << std::get<2>(v) << '\n';                   // by index

    v = 3.5;
    std::cout << "index = " << v.index()
              << ", value = " << std::get<double>(v) << '\n';           // 1, 3.5
    return 0;
}
