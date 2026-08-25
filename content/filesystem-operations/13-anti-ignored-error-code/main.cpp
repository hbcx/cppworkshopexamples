#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: passing an error_code to the non-throwing form and then
    // never looking at it. The operation can fail silently and the code marches
    // on using a bogus result -- worse than the exception you were trying to
    // avoid, because nothing tells you.
    fs::path missing = fs::temp_directory_path() / "cppws_absent_88.bin";

    std::error_code ec;
    std::uintmax_t size = fs::file_size(missing, ec);   // fails, ec is set

    // Ignoring ec, this looks like a real (enormous) size: on failure file_size
    // returns uintmax_t(-1).
    std::cout << "ignoring ec, size looks like: " << size << '\n'; // 18446744073709551615

    // Fix: check ec right after the call (or use the throwing form and catch).
    if (ec)
        std::cout << "checked ec: operation failed, do not trust the result\n";
    else
        std::cout << "size: " << size << '\n';
    return 0;
}
