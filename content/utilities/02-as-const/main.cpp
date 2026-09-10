#include <iostream>
#include <string>
#include <utility>

// Two overloads: the non-const one may modify; the const one is read-only.
void inspect(std::string& s) {
    s += "!";
    std::cout << "  non-const overload ran (modified)\n";
}
void inspect(const std::string& s) {
    std::cout << "  const overload ran (read-only, size " << s.size() << ")\n";
}

int main() {
    std::string name = "Ada";

    inspect(name);                 // non-const object -> picks the modifying overload
    std::cout << "after inspect(name):            " << name << "\n";

    inspect(std::as_const(name));  // a const view -> forces the read-only overload
    std::cout << "after inspect(as_const(name)):  " << name << "\n";
    return 0;
}
