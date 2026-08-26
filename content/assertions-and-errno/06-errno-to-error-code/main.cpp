// Bridge the C world to the C++ one: an errno value becomes a std::error_code in
// the generic category, which you can compare against std::errc, pass around, log
// or escalate into a std::system_error -- instead of threading a bare int and
// re-testing E* macros everywhere.
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <system_error>

// Wrap a C parse so callers get an error_code, not an errno convention.
std::error_code parse_long(const char* text, long& out) {
    errno = 0;
    char* end = nullptr;
    long v = std::strtol(text, &end, 10);
    if (end == text || *end != '\0') {
        return std::make_error_code(std::errc::invalid_argument);   // nothing / trailing junk
    }
    if (errno != 0) {
        return std::error_code(errno, std::generic_category());     // ERANGE -> result_out_of_range
    }
    out = v;
    return {};
}

int main() {
    std::cout << std::boolalpha;
    long value = 0;

    std::error_code ec = parse_long("999999999999999999999999", value);
    std::cout << "overflow is out_of_range: "
              << (ec == std::errc::result_out_of_range) << '\n';    // true

    ec = parse_long("abc", value);
    std::cout << "junk is invalid_argument: "
              << (ec == std::errc::invalid_argument) << '\n';       // true

    ec = parse_long("123", value);
    std::cout << "clean parse ok: " << (!ec && value == 123) << '\n'; // true
    return 0;
}
