#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_seek";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "data.bin";
    { std::ofstream(file, std::ios::binary) << "ABCDEFGHIJ"; }   // 10 bytes

    std::ifstream in(file, std::ios::binary);

    // Seek to the end, then tellg to get the size in bytes.
    in.seekg(0, std::ios::end);
    std::streamoff size = in.tellg();
    std::cout << "size via seek: " << size << '\n';   // 10

    // Jump to byte 4 and read three bytes -- random access, no scanning.
    in.seekg(4, std::ios::beg);
    char buf[4] = {};
    in.read(buf, 3);
    std::cout << "bytes 4..6: " << buf << '\n';   // EFG

    // The position advanced by what we read.
    std::cout << "now at: " << static_cast<std::streamoff>(in.tellg()) << '\n'; // 7

    fs::remove_all(dir, ec);
    return 0;
}
