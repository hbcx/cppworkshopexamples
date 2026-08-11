#include <array>
#include <iostream>

// A constexpr VARIABLE is a true compile-time constant. Unlike a plain const, it
// can be used wherever the language REQUIRES a constant expression.
constexpr int kSize = 8;

template <int N>
struct Fixed { int data[N]; };

int main() {
    int raw[kSize] = {};                 // array bound: needs a constant
    std::array<int, kSize> arr{};        // template non-type argument: needs a constant
    Fixed<kSize> fixed{};                // our own template argument
    static_assert(kSize == 8, "kSize is 8");   // static_assert: needs a constant

    switch (3) {
        case kSize - 5:                  // case label: needs a constant (kSize-5 == 3)
            std::cout << "case label used kSize\n";
            break;
        default:
            break;
    }

    std::cout << "raw holds "   << (sizeof(raw) / sizeof(raw[0]))          << " ints, "
              << "array holds " << arr.size()                              << ", "
              << "Fixed holds " << (sizeof(fixed.data) / sizeof(fixed.data[0])) << "\n";
    return 0;
}
