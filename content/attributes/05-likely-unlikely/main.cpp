#include <iostream>

// [[likely]] / [[unlikely]] tell the optimizer which path is the common one, so it
// can lay out the hot code straight-line and push the rare path aside. They change
// no results -- only code generation -- so they are a hint, not a guarantee.
long doubleNonNegative(int x) {
    if (x >= 0) [[likely]] {
        return x * 2L;
    } else [[unlikely]] {
        return -1;
    }
}

int classify(int code) {
    switch (code) {
        case 200: [[likely]] return 1;    // the overwhelmingly common case
        case 500: [[unlikely]] return -1;
        default: return 0;
    }
}

int main() {
    std::cout << "doubleNonNegative(21) = " << doubleNonNegative(21) << "\n";
    std::cout << "doubleNonNegative(-1) = " << doubleNonNegative(-1) << "\n";
    std::cout << "classify(200) = " << classify(200) << "\n";
    std::cout << "classify(500) = " << classify(500) << "\n";
    return 0;
}
