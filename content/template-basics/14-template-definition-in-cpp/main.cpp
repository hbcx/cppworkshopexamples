#include <iostream>
#include "mathutil.hpp"

int main() {
    // These link ONLY because mathutil.cpp explicitly instantiated int and double.
    std::cout << "twice(21)  = " << twice(21) << "\n";     // 42
    std::cout << "twice(2.5) = " << twice(2.5) << "\n";    // 5

    // twice('x');   // would fail to LINK: no explicit instantiation for char, and
    //               // the definition is not visible here to instantiate implicitly.
    std::cout << "done\n";
    return 0;
}
