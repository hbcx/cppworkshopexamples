#include <charconv>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // Anti-pattern: assuming to_chars always succeeds. When the buffer cannot
    // hold the number it returns value_too_large and writes nothing usable;
    // on failure ptr is set to the end of the range.
    char buf[4];

    int value = 123456;  // needs 6 characters, buffer holds at most 4
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof buf, value);

    // WRONG: computing a length from ptr without checking ec. On failure
    // ptr == end, so this "length" is the whole buffer and the bytes it covers
    // were never written -- printing them would be a silent bug, so we only
    // report the misleading length here.
    std::size_t wrong_len = static_cast<std::size_t>(ptr - buf);
    std::cout << "ignoring ec: claimed length = " << wrong_len
              << " (nothing valid was written)\n";

    // RIGHT: check ec first. On value_too_large, grow the buffer or report it.
    if (ec == std::errc{})
        std::cout << "ok: " << std::string_view(buf, ptr - buf) << '\n';
    else
        std::cout << "checking ec (right): buffer too small for " << value << '\n';
    return 0;
}
