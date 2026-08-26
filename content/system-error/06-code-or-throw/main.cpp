// error_code and exceptions are two ends of the same information. A function can
// RETURN an error_code (non-throwing, the caller checks it), or the caller can
// escalate a code into an exception by throwing std::system_error, which carries
// the code so a handler can still inspect it. Same error_code, two styles.
#include <iostream>
#include <system_error>

// Reports failure by returning a code instead of throwing.
std::error_code connect(bool reachable) {
    if (!reachable) return std::make_error_code(std::errc::host_unreachable);
    return {};   // success
}

int main() {
    std::cout << std::boolalpha;

    // Style 1: check the returned code and handle it in place.
    std::error_code ec = connect(false);
    if (ec) {
        std::cout << "returned code, unreachable: "
                  << (ec == std::errc::host_unreachable) << '\n';

        // Style 2: escalate the SAME code into an exception for a distant caller.
        try {
            throw std::system_error(ec, "connect to primary");
        }
        catch (const std::system_error& e) {
            std::cout << "thrown, same code: "
                      << (e.code() == std::errc::host_unreachable) << '\n';
        }
    }
    return 0;
}
