// The runtime_error children, for numeric results a computation cannot
// represent:
//   range_error     -- a result outside the range of the type
//   overflow_error  -- an arithmetic overflow
//   underflow_error -- a result too small to represent
// You throw these from your own numeric code; the library rarely does.
#include <iostream>
#include <stdexcept>
#include <limits>

// Multiply with an explicit overflow check instead of letting int wrap (which
// would be undefined behaviour for signed overflow).
int checked_mul(int a, int b) {
    long long wide = static_cast<long long>(a) * b;
    if (wide > std::numeric_limits<int>::max() ||
        wide < std::numeric_limits<int>::min()) {
        throw std::overflow_error("int multiplication overflowed");
    }
    return static_cast<int>(wide);
}

int main() {
    std::cout << "100 * 200 = " << checked_mul(100, 200) << '\n';

    try {
        (void)checked_mul(100000, 100000);      // 10^10 does not fit in int
    }
    catch (const std::runtime_error& e) {
        std::cout << "overflow_error: " << e.what() << '\n';
    }

    try {
        throw std::range_error("result outside representable range");
    }
    catch (const std::runtime_error& e) {
        std::cout << "range_error:    " << e.what() << '\n';
    }
    return 0;
}
