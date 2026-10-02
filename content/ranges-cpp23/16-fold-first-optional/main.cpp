#include <ranges>
#include <algorithm>
#include <vector>
#include <functional>
#include <optional>
#include <iostream>

// fold_left_first takes no initial value -- it seeds with the first element -- so
// an EMPTY range has no result. It returns std::optional, and for an empty range
// that optional is empty. Calling .value() or * on it is a bug.

int main() {
    std::vector<int> data{};   // empty, e.g. no rows matched a filter

    // BAD (shown in a comment -- it would throw std::bad_optional_access):
    //   int m = std::ranges::fold_left_first(data, std::plus{}).value();
    // For an empty range the optional is empty, so .value() throws at run time.

    // FIX 1: check the optional before using it.
    std::optional<int> sum = std::ranges::fold_left_first(data, std::plus{});
    if (sum)
        std::cout << "sum = " << *sum << '\n';
    else
        std::cout << "empty range -- no sum to report\n";

    // FIX 2: if a sensible starting value exists, use fold_left instead. It
    // always returns a value (the init) even for an empty range.
    int total = std::ranges::fold_left(data, 0, std::plus{});
    std::cout << "fold_left with init 0 = " << total << '\n';
    return 0;
}
