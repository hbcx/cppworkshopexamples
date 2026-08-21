#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>
#include <vector>

int main() {
    // A line of whitespace-separated numbers. We walk it with from_chars, using
    // the returned ptr as the next starting point -- no substr, no split into
    // std::strings, no allocation per number.
    std::string_view line = "12 34 -7 1000 42";

    const char* it = line.data();
    const char* end = line.data() + line.size();
    std::vector<int> numbers;

    while (it != end) {
        // from_chars does NOT skip whitespace, so step past separators by hand.
        while (it != end && *it == ' ')
            ++it;
        if (it == end)
            break;

        int value = 0;
        auto [ptr, ec] = std::from_chars(it, end, value);
        if (ec != std::errc{}) {
            std::cout << "stopped at a non-number\n";
            break;
        }
        numbers.push_back(value);
        it = ptr;  // continue exactly where this number ended
    }

    std::cout << "parsed " << numbers.size() << " numbers:";
    for (int n : numbers)
        std::cout << ' ' << n;
    std::cout << '\n';
    return 0;
}
