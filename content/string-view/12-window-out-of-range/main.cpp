#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string_view>

int main() {
    std::string_view sv = "abcdef";      // size 6

    // substr checks its POSITION: pos > size() throws std::out_of_range (defined).
    try {
        std::string_view bad = sv.substr(10);   // pos 10 > 6
        std::cout << bad;                        // not reached
    } catch (const std::out_of_range&) {
        std::cout << "substr(10) threw out_of_range\n";
    }

    // substr clamps the LENGTH, though: a count past the end is fine.
    std::cout << "substr(4, 100) = " << sv.substr(4, 100) << '\n';   // "ef"

    // WRONG (described, not run): remove_prefix / remove_suffix do NOT check.
    // Shrinking by more than size() is UNDEFINED behaviour, not an exception:
    //   sv.remove_prefix(99);      // undefined -- moves the pointer past the end
    // RIGHT: clamp the amount yourself before shrinking the window.
    std::size_t drop = 99;
    sv.remove_prefix(std::min(drop, sv.size()));
    std::cout << "after clamped remove_prefix, empty = " << std::boolalpha
              << sv.empty() << '\n';
    return 0;
}
