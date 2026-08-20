#include <iostream>
#include <string>
#include <string_view>

// Borrows its input (a view), returns something that OWNS its characters.
static std::string quote(std::string_view sv) {
    std::string s;
    s.reserve(sv.size() + 2);
    s.push_back('"');
    s.append(sv);            // std::string::append takes a string_view (C++17)
    s.push_back('"');
    return s;
}

int main() {
    std::string_view name = "ada";

    // std::string owned = name;   // ERROR: no implicit view -> string conversion
    std::string owned(name);       // explicit: this is where the copy happens
    std::cout << "owned = " << owned << " (size " << owned.size() << ")\n";

    // Materialize only at the boundary where ownership is actually needed.
    std::cout << quote("borrowed literal") << '\n';

    // Copying a view into an existing string, two equivalent ways.
    std::string box = "old";
    box.assign(name);              // or: box = std::string(name);
    std::cout << "box = " << box << '\n';
    return 0;
}
