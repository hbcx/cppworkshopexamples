// Comparing an error_code to a std::errc is PORTABLE: the == operator asks the
// categories whether the two are equivalent, so it works even when the code came
// from a platform-specific category. Always compare against errc, never against
// a raw number -- the number's meaning depends on the category and the OS.
#include <iostream>
#include <system_error>

// Pretend this came from some library call that reports via error_code.
std::error_code do_open() {
    return std::make_error_code(std::errc::no_such_file_or_directory);
}

int main() {
    std::cout << std::boolalpha;
    std::error_code ec = do_open();

    // Portable checks against named conditions:
    std::cout << "no_such_file: " << (ec == std::errc::no_such_file_or_directory) << '\n';
    std::cout << "permission:   " << (ec == std::errc::permission_denied) << '\n';

    // error_condition is the portable sibling: default_error_condition() maps a
    // code (possibly platform-specific) to the generic condition it means, which
    // is what the errc comparison uses underneath.
    std::error_condition cond = ec.default_error_condition();
    std::cout << "condition category: " << cond.category().name() << '\n';    // generic
    std::cout << "condition is enoent: "
              << (cond == std::errc::no_such_file_or_directory) << '\n';       // true
    return 0;
}
