#include <iostream>

import calc;   // sees only the interface, not the implementation unit

int main() {
    std::cout << "add(2, 3) = " << add(2, 3) << "\n";
    std::cout << "mul(4, 5) = " << mul(4, 5) << "\n";
    return 0;
}
