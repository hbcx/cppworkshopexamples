#include <iomanip>
#include <iostream>

int main() {
    int n = 255;
    // dec/hex/oct select the integer base; showbase adds the 0x/0 prefix;
    // uppercase capitalises the hex digits and the X.
    std::cout << "dec: " << std::dec << n << '\n';   // 255
    std::cout << "hex: " << std::hex << n << '\n';   // ff
    std::cout << "oct: " << std::oct << n << '\n';   // 377
    std::cout << "showbase hex: " << std::showbase << std::hex << n << '\n';  // 0xff
    std::cout << "uppercase:    " << std::uppercase << std::hex << n << '\n'; // 0XFF

    // Base and showbase are STICKY -- reset when you are done.
    std::cout << std::dec << std::nouppercase << std::noshowbase;
    std::cout << "back to dec: " << n << '\n';   // 255
    return 0;
}
