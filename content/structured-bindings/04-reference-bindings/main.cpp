#include <iostream>
#include <string>

// By default auto [a, b] copies the object and binds to the copy. Add & to bind
// to the ORIGINAL (so you can modify it in place), or const auto& for a
// read-only view with no copy -- the same choices as any auto declaration.

struct Item {
    std::string name;
    int         count;
};

int main() {
    Item item{"bolts", 10};

    // Reference binding: modify the real members through the names.
    auto& [name, count] = item;
    count += 5;
    name = "bolts (M6)";
    std::cout << "item is now " << item.name << " x" << item.count << "\n"; // bolts (M6) x15

    // const reference binding: read without copying, cannot modify.
    const auto& [cname, ccount] = item;
    // ccount = 0;              // error: bound through const&
    std::cout << "read-only view: " << cname << " x" << ccount << "\n";

    // Value binding: a copy; writing does not touch the source.
    auto [vname, vcount] = item;
    vcount = 0;
    vname.clear();
    std::cout << "after value-copy write, item.count still " << item.count << "\n"; // 15
    return 0;
}
