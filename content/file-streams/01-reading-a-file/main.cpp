#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_read";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "numbers.txt";
    { std::ofstream(file) << "10 20 30\n"; }   // set up a file to read

    // Open a file for reading with std::ifstream. Always check that it opened:
    // the stream converts to false when it is not usable.
    std::ifstream in(file);
    if (!in) {
        std::cout << "could not open file\n";
        return 1;
    }

    // operator>> reads formatted values, skipping the whitespace between them.
    int a = 0, b = 0, c = 0;
    in >> a >> b >> c;
    std::cout << "read: " << a << " " << b << " " << c << '\n';
    std::cout << "sum:  " << (a + b + c) << '\n';

    // The ifstream closes the file automatically at end of scope (RAII) -- no
    // explicit close() needed.
    fs::remove_all(dir, ec);
    return 0;
}
