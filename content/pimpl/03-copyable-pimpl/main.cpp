#include "config.hpp"

#include <iostream>

int main() {
    Config a;
    a.set("mode", "fast");

    Config b = a;          // deep copy: b gets its own Impl
    b.set("mode", "slow"); // changing the copy...

    // ...leaves the original untouched.
    std::cout << "a.mode = " << a.get("mode") << "\n";
    std::cout << "b.mode = " << b.get("mode") << "\n";
}
