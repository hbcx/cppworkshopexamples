#include <iostream>

// A structured binding can unpack a built-in array too: one name per element.
// The number of names must match the array's length exactly.

int main() {
    int rgb[3] = {200, 100, 50};
    auto [r, g, b] = rgb;        // three elements -> three names
    std::cout << "r=" << r << " g=" << g << " b=" << b << "\n";

    // With plain auto (no &) each name is a COPY of the element:
    r = 0;
    std::cout << "after r=0: r=" << r << ", rgb[0] still " << rgb[0] << "\n"; // 200

    // A reference binding shares the array's storage (see reference-bindings):
    auto& [r2, g2, b2] = rgb;
    r2 = 255;
    std::cout << "after r2=255 via ref: rgb[0] = " << rgb[0] << "\n";         // 255
    return 0;
}
