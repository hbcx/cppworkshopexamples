#include <array>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <string_view>

int main() {
    // Anti-pattern: to_chars does NOT write a terminating '\0'. Treating the
    // buffer as a C-string reads whatever bytes sit after the digits.

    // Fill the buffer with a visible marker and plant a terminator at the end,
    // so the WRONG output below is defined and reproducible. In real code these
    // bytes are indeterminate and there is no terminator at all -- reading them
    // as a C-string is undefined behaviour.
    std::array<char, 16> buf;
    buf.fill('#');
    buf.back() = '\0';

    int value = 42;
    auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), value);
    (void)ec;

    // WRONG: buf.data() is not null-terminated at ptr, so "%s" runs past "42".
    std::printf("as a C-string (wrong): %s\n", buf.data());

    // RIGHT: the written text is exactly [buf, ptr). Carry the length.
    std::string_view text(buf.data(), ptr - buf.data());
    std::cout << "using ptr (right):     " << text << '\n';
    return 0;
}
