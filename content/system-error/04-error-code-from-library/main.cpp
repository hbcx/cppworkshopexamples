// The standard library offers non-throwing overloads that report through an
// error_code you pass in: <filesystem> operations take an error_code& and set it
// instead of throwing. You check the code and decide -- no try/catch on a
// routine "file not there".
#include <iostream>
#include <system_error>
#include <filesystem>
#include <cstdint>

namespace fs = std::filesystem;

int main() {
    std::cout << std::boolalpha;
    std::error_code ec;

    // A path that does not exist. The throwing overload would throw here; this
    // one just sets ec.
    fs::path missing = fs::temp_directory_path() / "cppws-does-not-exist-42.tmp";
    std::uintmax_t size = fs::file_size(missing, ec);

    std::cout << "call failed: " << static_cast<bool>(ec) << '\n';                    // true
    std::cout << "is not-found: " << (ec == std::errc::no_such_file_or_directory) << '\n'; // true

    // On failure file_size returns the documented sentinel, not a real size --
    // which is why you must check the code, not the return value.
    std::cout << "returned the -1 sentinel: "
              << (size == static_cast<std::uintmax_t>(-1)) << '\n';                   // true
    return 0;
}
