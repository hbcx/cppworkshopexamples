// Anti-pattern: using assert to validate EXTERNAL input -- user data, a file, a
// network message. asserts compile out under NDEBUG, so in a release build the
// check is gone and the bad input flows straight through, causing in production
// exactly the corruption the assert seemed to prevent. Validate input with real
// code (if + error_code or throw); keep assert for your own internal invariants.
#define NDEBUG                 // release build: asserts are gone
#include <cassert>
#include <iostream>
#include <string>
#include <system_error>

// WRONG: "validate" a user-supplied index with assert. Under NDEBUG the assert is
// removed, so this reads s[i] even when i is out of range. Never called here.
int element_wrong(const std::string& s, int i) {
    assert(i >= 0 && i < static_cast<int>(s.size()) && "index in range");
    return s[static_cast<std::size_t>(i)];
}

// RIGHT: validate with real code that runs in every build.
std::error_code element_right(const std::string& s, int i, char& out) {
    if (i < 0 || i >= static_cast<int>(s.size())) {
        return std::make_error_code(std::errc::result_out_of_range);
    }
    out = s[static_cast<std::size_t>(i)];
    return {};
}

int main() {
    std::cout << std::boolalpha;
    std::string s = "abc";

    // element_wrong(s, 5) is NOT called: under NDEBUG its assert is gone, so it
    // would read s[5] out of range -- undefined behaviour, the very bug the assert
    // pretended to guard. That is the point: the guard vanished in release.
    std::cout << "wrong: the assert guard is compiled out under NDEBUG\n";

    // element_right validates in every build and reports the bad index.
    char c = '?';
    std::error_code ec = element_right(s, 5, c);
    std::cout << "right: out_of_range = " << (ec == std::errc::result_out_of_range) << '\n';

    ec = element_right(s, 1, c);
    std::cout << "right: s[1] = " << c << ", ok = " << !ec << '\n';
    return 0;
}
