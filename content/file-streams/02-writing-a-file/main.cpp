#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_write";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "out.txt";

    // std::ofstream opens a file for writing. By default it CREATES the file, or
    // TRUNCATES it to empty if it already exists.
    {
        std::ofstream out(file);
        if (!out) { std::cout << "open failed\n"; return 1; }
        out << "line one\n";
        out << "value = " << 42 << '\n';
    }   // out's destructor flushes and closes here

    // Read it back to show it was written.
    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line))
        std::cout << "got: " << line << '\n';

    fs::remove_all(dir, ec);
    return 0;
}
