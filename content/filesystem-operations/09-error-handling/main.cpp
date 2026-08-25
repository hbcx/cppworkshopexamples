#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path missing = fs::temp_directory_path() / "cppws_no_such_file_qzx.dat";

    // Most operations come in TWO forms. The throwing form reports failure with
    // a filesystem_error exception:
    try {
        std::uintmax_t s = fs::file_size(missing);
        std::cout << "unexpected size: " << s << '\n';
    } catch (const fs::filesystem_error&) {
        std::cout << "throwing form: caught filesystem_error\n";
    }

    // The other form takes a std::error_code&: no exception, the error is
    // reported in the code. Good on a hot path or when failure is expected.
    std::error_code ec;
    std::uintmax_t s = fs::file_size(missing, ec);
    std::cout << "error_code form failed: " << static_cast<bool>(ec) << '\n';   // 1
    std::cout << "sentinel size returned: "
              << (s == static_cast<std::uintmax_t>(-1)) << '\n';                // 1

    // On success the same ec is cleared back to no-error.
    ec.clear();
    bool present = fs::exists(fs::temp_directory_path(), ec);
    std::cout << "cleared on success: " << (present && !ec) << '\n';   // 1
    return 0;
}
