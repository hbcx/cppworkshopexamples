#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_ws";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "people.txt";
    { std::ofstream(file) << "Grace Hopper\n42\n"; }

    // Anti-pattern 1: using operator>> to read a name with a space in it. >>
    // stops at the first whitespace, so it grabs only "Grace".
    {
        std::ifstream in(file);
        std::string name;
        in >> name;
        std::cout << "extracted with >>: " << name << '\n';   // Grace
    }

    // Anti-pattern 2: mixing >> and getline. After >> reads the number it leaves
    // the '\n' in the stream, so the next getline returns an EMPTY line.
    {
        std::ifstream in(file);
        std::string firstLine;
        std::getline(in, firstLine);      // "Grace Hopper" -- a whole line, correct
        int n = 0;
        in >> n;                          // 42, leaves the trailing '\n'
        std::string leftover;
        std::getline(in, leftover);       // EMPTY: consumes the dangling newline
        std::cout << "line: " << firstLine << "  n: " << n
                  << "  leftover empty: " << leftover.empty() << '\n';
    }

    // Fix: read lines with getline, and after a >> skip to the next line with
    // std::ws (or ignore) before calling getline.
    fs::remove_all(dir, ec);
    return 0;
}
