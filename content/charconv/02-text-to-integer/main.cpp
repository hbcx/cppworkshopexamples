#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // from_chars reads a number out of a character range. Like to_chars it does
    // not allocate, does not throw, and is locale-independent (always '.').
    std::string_view text = "2048";

    int value = 0;  // left untouched if nothing parses -- always initialise it.
    auto result = std::from_chars(text.data(), text.data() + text.size(), value);

    // Two things say whether it worked:
    //   ec  == std::errc{}  -> a number was read with no error
    //   ptr                 -> one past the last character consumed
    if (result.ec == std::errc{}) {
        std::cout << "parsed: " << value << '\n';

        // Did it use the whole string, or stop partway?
        bool whole = result.ptr == text.data() + text.size();
        std::cout << "consumed the whole string: " << whole << '\n';
    } else {
        std::cout << "parse failed\n";
    }
    return 0;
}
