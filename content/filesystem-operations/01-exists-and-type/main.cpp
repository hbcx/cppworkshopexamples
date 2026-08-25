#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // The query free functions ask the disk what is there. Set up a sandbox with
    // one file and one subdirectory, then ask about each.
    fs::path root = fs::temp_directory_path() / "cppws_fsq";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "sub");
    std::ofstream(root / "data.txt") << "hello";

    fs::path file = root / "data.txt";
    fs::path dir  = root / "sub";
    fs::path gone = root / "missing.txt";

    std::cout << "exists(file):          " << fs::exists(file) << '\n';         // 1
    std::cout << "is_regular_file(file): " << fs::is_regular_file(file) << '\n'; // 1
    std::cout << "is_directory(file):    " << fs::is_directory(file) << '\n';   // 0
    std::cout << "is_directory(dir):     " << fs::is_directory(dir) << '\n';    // 1
    std::cout << "exists(missing):       " << fs::exists(gone) << '\n';         // 0

    // status() returns the file_type once, which you can compare -- cheaper than
    // several is_* calls that each stat the file.
    fs::file_status st = fs::status(file);
    std::cout << "type is regular:       " << (st.type() == fs::file_type::regular) << '\n'; // 1

    fs::remove_all(root, ec);
    return 0;
}
