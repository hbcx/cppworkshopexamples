#include <iostream>

import geo;   // one import; both the primary and the re-exported partition are visible

int main() {
    std::cout << "rectangleArea(4, 5) = " << rectangleArea(4.0, 5.0) << "\n";  // 20
    std::cout << "triangleArea(6, 3)  = " << triangleArea(6.0, 3.0) << "\n";   // 9
    return 0;
}
