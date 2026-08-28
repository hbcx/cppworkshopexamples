#include <iostream>
#include <limits>

// Store a value into a byte-sized field.
unsigned char toByte(int value) {
    // BAD: the compiler warned that int -> unsigned char may lose data. Wrapping
    // the conversion in a cast silences the warning but keeps the loss.
    return static_cast<unsigned char>(value);
}

// GOOD: the cast is fine once the range is guaranteed. Check first, then convert.
bool toByteChecked(int value, unsigned char& out) {
    if (value < 0 || value > std::numeric_limits<unsigned char>::max())
        return false;                             // reject what would not fit
    out = static_cast<unsigned char>(value);      // now the cast cannot lose data
    return true;
}

int main() {
    // The cast hid the problem: 300 does not fit in a byte and wraps to 44.
    std::cout << "toByte(300) = " << static_cast<int>(toByte(300))
              << "  (data lost -- 300 wrapped)\n";        // 44

    unsigned char b = 0;
    if (toByteChecked(300, b))
        std::cout << "checked: stored " << static_cast<int>(b) << "\n";
    else
        std::cout << "checked: 300 does not fit in a byte -- rejected\n";   // this line

    if (toByteChecked(200, b))
        std::cout << "checked: stored " << static_cast<int>(b) << "\n";     // 200
    return 0;
}
