#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_whole";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "blob.txt";
    { std::ofstream(file, std::ios::binary) << "abc\ndef\n"; }   // 8 bytes

    // Read the ENTIRE file into one std::string. The streambuf-iterator range
    // copies every byte in one go. The extra parentheses around the first
    // argument avoid the most-vexing-parse. Binary mode keeps the byte count
    // exact.
    std::ifstream in(file, std::ios::binary);
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    std::cout << "bytes: " << contents.size() << '\n';   // 8
    std::cout << contents;                                // abc / def

    // The stringstream alternative reads the same bytes via rdbuf():
    std::ifstream in2(file, std::ios::binary);
    std::ostringstream ss;
    ss << in2.rdbuf();
    std::cout << "rdbuf bytes: " << ss.str().size() << '\n';   // 8

    fs::remove_all(dir, ec);
    return 0;
}
