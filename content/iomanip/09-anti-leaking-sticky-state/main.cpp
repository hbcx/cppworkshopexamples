#include <iomanip>
#include <iostream>

// A helper that formats a value as hex -- but forgets to restore the stream.
static void printHexBad(int n) {
    std::cout << std::hex << n;
}

int main() {
    // Anti-pattern: a function sets a sticky manipulator on a shared stream and
    // does not restore it, so it silently changes formatting for everything
    // printed AFTER it -- a bug that surfaces far from its cause.
    std::cout << "before: " << 255 << '\n';   // 255 (decimal)

    printHexBad(255);
    std::cout << '\n';
    std::cout << "after bad helper: " << 255 << '\n';   // ff  <- still hex! leaked

    // Reset what leaked.
    std::cout << std::dec;
    // Fix: the helper should restore the stream state before returning (save
    // flags, or copyfmt from a default stream).
    std::cout << "after dec reset: " << 255 << '\n';   // 255
    return 0;
}
