#include <iomanip>
#include <iostream>

int main() {
    // setw sets the field WIDTH for the next output; setfill sets the padding
    // character; left/right/internal choose alignment. Together they make
    // fixed-width columns.
    std::cout << '[' << std::setw(8) << "left" << "]\n";               // [    left]  (right by default)
    std::cout << '[' << std::left << std::setw(8) << "left" << "]\n";  // [left    ]
    std::cout << '[' << std::right << std::setw(8) << 42 << "]\n";     // [      42]
    std::cout << '[' << std::setfill('.') << std::setw(8) << 42 << "]\n"; // [......42]

    // internal puts the fill BETWEEN the sign and the digits.
    std::cout << '[' << std::internal << std::setfill('0')
              << std::setw(8) << -42 << "]\n";                         // [-0000042]
    return 0;
}
