// std::system_error (from <system_error>) is part of this hierarchy too: it
// derives from std::runtime_error and adds an error code describing an operating
// system or library failure. So a generic catch (const std::runtime_error&)
// still catches it, but catching std::system_error lets you inspect code().
#include <iostream>
#include <stdexcept>
#include <system_error>

int main() {
    // The base handler catches it like any other runtime_error...
    try {
        throw std::system_error(
            std::make_error_code(std::errc::permission_denied), "open config");
    }
    catch (const std::runtime_error&) {
        std::cout << "as runtime_error: caught a system failure\n";
    }

    // ...the specific type exposes the machine-readable code and its category.
    try {
        throw std::system_error(
            std::make_error_code(std::errc::no_such_file_or_directory), "read cache");
    }
    catch (const std::system_error& e) {
        std::cout << "category: " << e.code().category().name() << '\n';
        std::cout << "is no_such_file: " << std::boolalpha
                  << (e.code() == std::errc::no_such_file_or_directory) << '\n';
    }
    return 0;
}
