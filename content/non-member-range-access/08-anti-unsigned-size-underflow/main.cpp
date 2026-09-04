#include <iostream>
#include <vector>
#include <iterator>   // std::size, std::ssize (C++20)
#include <cstddef>    // std::ptrdiff_t

int main() {
    std::vector<int> v;   // empty on purpose

    // ANTI-PATTERN: std::size returns an UNSIGNED type. On an empty range,
    // size() - 1 does not become -1; it wraps to the largest std::size_t.
    auto last_unsigned = std::size(v) - 1;
    std::cout << "unsigned size()-1 on empty = " << last_unsigned << "\n";

    // A loop written as  for (std::size_t i = 0; i <= std::size(v) - 1; ++i)
    // would then run billions of times over an empty vector -- a classic bug.
    // It is defined behaviour (modular wraparound), just wrong, so it is only
    // described here, not run.

    // FIX: std::ssize returns a SIGNED count, so the arithmetic behaves.
    std::ptrdiff_t last_signed = std::ssize(v) - 1;   // -1, as expected
    std::cout << "signed ssize()-1 on empty  = " << last_signed << "\n";

    int body_runs = 0;
    for (std::ptrdiff_t i = std::ssize(v) - 1; i >= 0; --i)
        ++body_runs;
    std::cout << "reverse loop body ran " << body_runs << " times\n";
    return 0;
}
