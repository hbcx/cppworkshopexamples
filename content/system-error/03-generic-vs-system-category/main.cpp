// Two built-in categories:
//   generic_category -- portable error conditions, the errc enumerators (POSIX
//                       names). Use it for errors you raise yourself.
//   system_category  -- the native operating-system error numbers (errno on
//                       POSIX, GetLastError on Windows). The library uses it for
//                       OS call failures.
// Their value() numbers and message() text are platform-specific, so never
// hard-code a number or match on the text -- compare against errc, which bridges
// the two.
#include <iostream>
#include <system_error>

int main() {
    std::cout << "generic name: " << std::generic_category().name() << '\n';   // generic
    std::cout << "system name:  " << std::system_category().name() << '\n';     // system

    std::cout << std::boolalpha;

    // errc lives in the generic category:
    std::error_code g = std::make_error_code(std::errc::invalid_argument);
    std::cout << "g in generic: " << (g.category() == std::generic_category()) << '\n';   // true
    std::cout << "g is invalid_argument: " << (g == std::errc::invalid_argument) << '\n'; // true

    // Categories are identity-compared by ADDRESS: each accessor returns a
    // reference to one global instance.
    std::cout << "generic != system: "
              << (std::generic_category() != std::system_category()) << '\n';   // true
    return 0;
}
