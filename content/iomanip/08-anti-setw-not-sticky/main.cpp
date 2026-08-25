#include <iomanip>
#include <iostream>

int main() {
    // Anti-pattern: setting setw once and expecting every field to be that wide.
    // setw applies to the NEXT output only, then resets to 0 -- so only the first
    // value is padded and the rest are jammed together.
    std::cout << "wrong:\n";
    std::cout << std::setw(6);
    std::cout << 1 << 2 << 3 << '\n';   // "     123" -- only the 1 is padded

    // Fix: re-apply setw before EVERY field you want padded.
    std::cout << "right:\n";
    std::cout << std::setw(6) << 1
              << std::setw(6) << 2
              << std::setw(6) << 3 << '\n';   // "     1     2     3"
    return 0;
}
