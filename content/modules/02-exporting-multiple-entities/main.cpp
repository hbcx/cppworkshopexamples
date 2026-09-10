#include <iostream>

import shapes;

int main() {
    Point a{1, 2}, b{4, 6};
    std::cout << "manhattan(a, b)   = " << manhattan(a, b) << "\n";
    std::cout << "dimensions        = " << dimensions << "\n";
    std::cout << "originDistance(a) = " << originDistance(a) << "\n";
    std::cout << "library_name()    = " << library_name() << "\n";
    return 0;
}
