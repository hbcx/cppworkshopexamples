#include <iostream>

import greeter;

int main() {
    // Works because greet() is exported. Had the export been forgotten, this call
    // would fail to compile with 'greet' was not declared -- the most common
    // first-day module mistake.
    std::cout << greet() << "\n";
    return 0;
}
