#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fsmeta";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    fs::path f = root / "data.bin";
    std::ofstream(f) << "abcdef";   // 6 bytes

    // file_size reports the byte length.
    std::cout << "size: " << fs::file_size(f) << '\n';   // 6

    // resize_file grows or shrinks the file. Growing pads with zero bytes.
    fs::resize_file(f, 10);
    std::cout << "after resize: " << fs::file_size(f) << '\n';   // 10

    // last_write_time reads AND writes the modification time. The value is a
    // file_time_type -- its own clock, not system_clock (see the anti-pattern).
    // Compare file times against each other, which stays on one clock.
    fs::file_time_type t = fs::last_write_time(f);
    fs::last_write_time(f, t + std::chrono::hours(1));   // stamp it one hour later
    fs::file_time_type t2 = fs::last_write_time(f);
    std::cout << "moved forward: " << (t2 > t) << '\n';   // 1

    fs::remove_all(root, ec);
    return 0;
}
