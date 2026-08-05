#include "document.hpp"

#include <iostream>
#include <utility>

int main() {
    Document a("Report");
    a.append("...contents...");

    Document b = std::move(a); // cheap: steals the unique_ptr, no Impl copied

    std::cout << "a moved-from? " << (a.empty() ? "yes" : "no") << "\n";
    std::cout << "b.title = " << b.title() << "\n";
}
