// std::error_code is a lightweight, non-throwing way to report an error: a
// numeric value plus a CATEGORY that says what the number means. A default one
// holds 0 in the system category and means "no error"; test it with its bool
// conversion. make_error_code turns a portable std::errc into an error_code.
#include <iostream>
#include <system_error>

int main() {
    std::cout << std::boolalpha;

    std::error_code ok;                                 // no error
    std::cout << "default is error: " << static_cast<bool>(ok) << '\n';   // false

    std::error_code ec = std::make_error_code(std::errc::permission_denied);
    std::cout << "ec is error: " << static_cast<bool>(ec) << '\n';        // true
    std::cout << "category: " << ec.category().name() << '\n';            // generic

    // Compare against a portable errc, never against a hard-coded number: the
    // number's meaning depends on the category.
    std::cout << "is permission_denied: "
              << (ec == std::errc::permission_denied) << '\n';            // true
    return 0;
}
