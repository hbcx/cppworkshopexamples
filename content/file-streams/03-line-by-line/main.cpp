#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_lines";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "poem.txt";
    { std::ofstream(file) << "the first line\nthe second line\nthe third line\n"; }

    // The correct read loop: getline in the condition. It reads one line per
    // pass (spaces included), consumes the newline, and stops the moment a read
    // fails -- no extra pass at end of file.
    std::ifstream in(file);
    std::string line;
    int n = 0;
    while (std::getline(in, line)) {
        ++n;
        std::cout << n << ": " << line << '\n';
    }
    std::cout << "lines: " << n << '\n';

    fs::remove_all(dir, ec);
    return 0;
}
