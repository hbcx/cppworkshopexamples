#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_binmode";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "data.bin";

    // Anti-pattern: opening in TEXT mode (the default) for BINARY data. On
    // Windows text mode translates the newline byte 0x0A to the two bytes
    // 0x0D 0x0A on write and back on read, so:
    //   - byte counts and file sizes are wrong for binary data,
    //   - a 0x0A byte inside the data gets a stray 0x0D added,
    //   - seek offsets computed from sizes do not line up.
    // (On Linux/macOS text mode does nothing, so the bug hides until Windows.)
    //
    // Fix: always pass std::ios::binary for non-text data.
    unsigned char data[] = { 0x00, 0x0A, 0xFF, 0x0A, 0x42 };   // contains 0x0A
    {
        std::ofstream out(file, std::ios::binary);
        out.write(reinterpret_cast<const char*>(data), sizeof data);
    }

    // Exactly 5 bytes on disk -- no 0x0A got expanded.
    std::cout << "binary file size: " << fs::file_size(file) << '\n';   // 5

    std::ifstream in(file, std::ios::binary);
    std::string back((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
    std::cout << "bytes read: " << back.size() << '\n';                 // 5
    std::cout << "byte 1 is 0x0A: "
              << (static_cast<unsigned char>(back[1]) == 0x0A) << '\n'; // 1

    fs::remove_all(dir, ec);
    return 0;
}
