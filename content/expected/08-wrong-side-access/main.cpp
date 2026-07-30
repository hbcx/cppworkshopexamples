#include <expected>
#include <iostream>
#include <string>

std::expected<int, std::string> parse(const std::string& s) {
    if (s == "7") return 7;
    return std::unexpected("not seven");
}

int main() {
    auto good = parse("7");
    auto bad = parse("x");

    // Anti-pattern: expected has TWO unchecked accessors, each valid only on its
    // own side. Using the wrong one is undefined behaviour. Not run:
    //
    //     *bad;            // UB: no value on the error side
    //     good.error();    // UB: no error on the value side
    //
    // Nothing warns you -- like optional's *, they trust that you checked first.

    // Fix: branch on has_value (or the bool conversion), then reach for the side
    // that is actually there.
    if (good) std::cout << "good -> value " << *good << '\n';
    else      std::cout << "good -> error " << good.error() << '\n';

    if (bad) std::cout << "bad -> value " << *bad << '\n';
    else     std::cout << "bad -> error " << bad.error() << '\n';

    // For the value side, value() converts a wrong assumption into a thrown
    // bad_expected_access (carrying the error) instead of silent UB.
    try {
        (void) bad.value();
    } catch (const std::bad_expected_access<std::string>& e) {
        std::cout << "value() on error threw, carrying: " << e.error() << '\n';
    }
    return 0;
}
