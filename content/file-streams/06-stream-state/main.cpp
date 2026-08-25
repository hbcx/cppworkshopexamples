#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_state";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "mixed.txt";
    { std::ofstream(file) << "42 oops"; }   // an int, then a non-number

    std::ifstream in(file);

    // A good read: 42 parses fine, the stream stays usable.
    int x = 0;
    in >> x;
    std::cout << "first read ok: " << static_cast<bool>(in)
              << " value " << x << '\n';                 // 1, 42

    // A failing read: "oops" is not an int, so failbit is set and (since C++11)
    // the target is written to 0.
    int y = -1;
    in >> y;
    std::cout << "second read ok: " << static_cast<bool>(in) << '\n'; // 0
    std::cout << "failbit: " << in.fail() << " value now: " << y << '\n'; // 1, 0

    // clear() resets the state so you can recover -- here, read the token as text.
    in.clear();
    std::string word;
    in >> word;
    std::cout << "after clear, read as text: " << word << '\n';   // oops

    // Alternatively you can make a stream THROW instead of checking bits:
    //   in.exceptions(std::ios::failbit | std::ios::badbit);

    fs::remove_all(dir, ec);
    return 0;
}
