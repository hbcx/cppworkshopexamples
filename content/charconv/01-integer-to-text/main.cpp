#include <array>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // to_chars writes the digits of a number into a buffer the caller owns.
    // It never allocates, never throws, and always uses '.' with no grouping --
    // the result does not depend on the global locale.
    std::array<char, 16> buf{};

    int value = -4096;

    // Write into [begin, end). to_chars returns { ptr, ec }:
    //   ptr -> one past the last character written
    //   ec  -> a std::errc, equal to std::errc{} on success
    auto result = std::to_chars(buf.data(), buf.data() + buf.size(), value);

    // There is NO terminating '\0'. The written text is exactly [buf, ptr).
    // Build the view from the returned ptr -- do not treat buf as a C-string.
    std::string_view text(buf.data(), result.ptr - buf.data());

    std::cout << "value as text:      " << text << '\n';
    std::cout << "characters written: " << text.size() << '\n';
    std::cout << "success:            " << (result.ec == std::errc{}) << '\n';
    return 0;
}
