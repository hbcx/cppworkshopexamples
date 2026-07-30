#include <iostream>
#include <variant>

// A type with no default constructor.
struct Connection {
    int fd;
    explicit Connection(int f) : fd(f) {}
};

int main() {
    std::cout << std::boolalpha;

    // A default-constructed variant holds its FIRST alternative. If that type is
    // not default-constructible, neither is the variant -- unless the first
    // alternative is std::monostate, an empty placeholder for "no value yet".
    std::variant<std::monostate, Connection> slot;   // starts empty (monostate)
    std::cout << "index = " << slot.index() << '\n';                            // 0
    std::cout << "empty? " << std::holds_alternative<std::monostate>(slot) << '\n';

    // Later, assign a real Connection.
    slot = Connection{5};
    std::cout << "empty? " << std::holds_alternative<std::monostate>(slot) << '\n';
    if (auto* c = std::get_if<Connection>(&slot)) {
        std::cout << "fd = " << c->fd << '\n';
    }
    return 0;
}
