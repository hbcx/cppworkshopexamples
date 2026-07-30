#include <expected>
#include <iostream>

// Anti-pattern: value type and error type are the SAME. expected<int, int>
// compiles, but a bare int like 5 is a VALUE, and only std::unexpected(5) makes
// it an ERROR -- so the two channels look identical and are easy to swap.
std::expected<int, int> classify(int x) {
    if (x >= 0) return x;              // value
    return std::unexpected(-x);        // error code (positive magnitude)
}

int main() {
    std::cout << std::boolalpha;

    for (int x : {7, -3}) {
        auto r = classify(x);
        // You must remember which int is which: *r is the value, r.error() the
        // error, and only has_value() tells them apart -- the type cannot.
        if (r) std::cout << x << " -> value " << *r << '\n';
        else   std::cout << x << " -> error code " << r.error() << '\n';
    }

    // Both hold the int 5, but mean opposite things -- indistinguishable by type.
    std::expected<int, int> asValue = 5;
    std::expected<int, int> asError = std::unexpected(5);
    std::cout << "asValue.has_value()=" << asValue.has_value()
              << ", asError.has_value()=" << asError.has_value() << '\n';

    // Fix: give the error channel a DISTINCT type -- an enum class or a struct --
    // so value and error can never be confused and error() is self-describing.
    return 0;
}
