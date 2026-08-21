#include <array>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <string_view>

int main() {
    std::array<char, 64> buf{};

    // A double cannot hold 0.1 exactly. The question when saving it to text is:
    // what string reads back to the identical double?
    double original = 0.1;

    // to_chars with no format/precision gives the SHORTEST string that round-
    // trips: the fewest digits that read back to the same double.
    auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), original);
    (void)ec;
    std::string_view shortest(buf.data(), ptr - buf.data());
    std::cout << "shortest round-trip text: " << shortest << '\n';

    // Read it back and confirm we recovered the identical value.
    double restored = 0;
    std::from_chars(shortest.data(), shortest.data() + shortest.size(), restored);
    std::cout << "restored == original:     " << (restored == original) << '\n';

    // The old habit: guess a precision. 17 digits always round-trip a double,
    // but for 0.1 that is longer and no more accurate than "0.1".
    char wide[64];
    std::snprintf(wide, sizeof wide, "%.17g", original);
    std::cout << "%.17g gives:              " << wide << " (longer, same value)\n";
    return 0;
}
