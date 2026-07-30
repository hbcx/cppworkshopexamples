#include <expected>
#include <iostream>
#include <string>

// An operation that either succeeds (nothing to return) or fails with a reason.
// expected<void, E> is the "did it work, and if not why" result.
std::expected<void, std::string> writeConfig(int value) {
    if (value < 0) return std::unexpected("value must be non-negative");
    // ... pretend to write it ...
    return {};   // success: an engaged expected<void, E> holds no value
}

int main() {
    std::cout << std::boolalpha;
    for (int v : {5, -1}) {
        auto r = writeConfig(v);
        if (r) std::cout << "write(" << v << "): ok\n";
        else   std::cout << "write(" << v << "): failed -- " << r.error() << '\n';
    }

    // has_value() still reports success; there is simply no value to read --
    // only success/failure and, on failure, the error.
    auto r = writeConfig(3);
    std::cout << "engaged? " << r.has_value() << '\n';
    return 0;
}
