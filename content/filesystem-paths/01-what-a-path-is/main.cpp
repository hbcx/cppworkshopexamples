#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    // A std::filesystem::path is just a value that HOLDS a pathname. Building
    // one touches no disk -- the file need not exist. You can make one from a
    // string literal, a std::string, or another path.
    fs::path fromLiteral = "docs/report.txt";
    std::string text = "images/logo.png";
    fs::path fromString{text};

    // Print with generic_string() so the separator is '/' on every platform
    // (native() would use '\\' on Windows). Same path, portable spelling.
    std::cout << "fromLiteral: " << fromLiteral.generic_string() << '\n';
    std::cout << "fromString:  " << fromString.generic_string() << '\n';

    // Two paths spelled the same compare equal, and a default path is empty.
    fs::path same = "docs/report.txt";
    std::cout << "equal to fromLiteral: " << (fromLiteral == same) << '\n';

    fs::path nothing;
    std::cout << "default path empty:   " << nothing.empty() << '\n';
    std::cout << "has a filename:       " << fromLiteral.has_filename() << '\n';
    return 0;
}
