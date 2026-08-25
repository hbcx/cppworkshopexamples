#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    // A stringstream plus manipulators builds a FORMATTED string: zero-padded
    // numbers, fixed precision, hex. The manipulators live in <iomanip>/<ios>.
    std::ostringstream id;
    id << "ID-" << std::setw(5) << std::setfill('0') << 42;
    std::cout << id.str() << '\n';   // ID-00042

    std::ostringstream price;
    price << std::fixed << std::setprecision(2) << 3.5;
    std::cout << price.str() << '\n';   // 3.50

    std::ostringstream code;
    code << "0x" << std::hex << std::uppercase << 255;
    std::cout << code.str() << '\n';   // 0xFF

    // One formatted field this way is fine; for many, the formatting chapter
    // (iomanip) and std::format (C++20) are clearer.
    return 0;
}
