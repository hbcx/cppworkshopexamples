#include <iostream>
#include <string>
#include <utility>
#include <vector>

int copies = 0;

// An element whose copy constructor counts, so we can see unwanted copies.
struct Big {
    std::string data;
    explicit Big(std::string d) : data(std::move(d)) {}
    Big(const Big& o) : data(o.data) { ++copies; }
    Big& operator=(const Big&) = default;
};

const std::vector<Big>& registry() {
    static std::vector<Big> v = { Big("alpha"), Big("beta"), Big("gamma") };
    return v;
}

int main() {
    registry();          // trigger the one-time static init BEFORE counting
    copies = 0;

    // BAD: `auto` deduces a VALUE, so this copies every element of the vector.
    for (auto item : registry()) {
        (void) item.data;
    }
    std::cout << "auto:        " << copies << " copies\n";   // 3

    // GOOD: bind a reference -- no copies.
    copies = 0;
    for (const auto& item : registry()) {
        (void) item.data;
    }
    std::cout << "const auto&: " << copies << " copies\n";   // 0
    return 0;
}
