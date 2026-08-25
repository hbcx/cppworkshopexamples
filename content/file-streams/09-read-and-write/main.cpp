#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_rw";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "slots.bin";
    { std::ofstream(file, std::ios::binary) << "0000000000"; }   // must exist first

    // in|out opens one stream for both reading and writing (binary for bytes).
    std::fstream io(file, std::ios::in | std::ios::out | std::ios::binary);

    // Overwrite bytes 3..5 in place -- the rest of the file is untouched.
    io.seekp(3, std::ios::beg);
    io.write("XYZ", 3);

    // Seek back to the start and read the whole thing to see the edit.
    io.seekg(0, std::ios::beg);
    std::string all((std::istreambuf_iterator<char>(io)),
                    std::istreambuf_iterator<char>());
    std::cout << "after in-place edit: " << all << '\n';   // 000XYZ0000

    fs::remove_all(dir, ec);
    return 0;
}
