#include <iomanip>
#include <ios>
#include <iostream>

int main() {
    // Manipulators are sticky, so a function that formats output should leave the
    // stream as it found it. Save the relevant state, do the formatting, restore.
    std::ios::fmtflags savedFlags = std::cout.flags();
    std::streamsize savedPrec = std::cout.precision();
    char savedFill = std::cout.fill();

    std::cout << std::hex << std::setfill('0') << std::setw(4) << 255 << '\n';   // 00ff

    // Restore everything so later output is unaffected.
    std::cout.flags(savedFlags);
    std::cout.precision(savedPrec);
    std::cout.fill(savedFill);

    std::cout << "restored: " << 255 << '\n';   // 255 (decimal, default fill)

    // To reset ALL formatting at once, copyfmt from a fresh stream:
    //   std::cout.copyfmt(std::ios(nullptr));
    return 0;
}
