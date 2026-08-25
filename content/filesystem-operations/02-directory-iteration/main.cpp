#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fsdir";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    std::ofstream(root / "beta.txt")  << "b";
    std::ofstream(root / "alpha.txt") << "aa";
    std::ofstream(root / "gamma.log") << "ccc";

    // directory_iterator walks the entries of ONE directory. Each entry is a
    // directory_entry that already knows its path and type (it cached them),
    // so entry.is_regular_file() needs no extra disk call.
    std::vector<std::string> names;
    for (const fs::directory_entry& entry : fs::directory_iterator(root))
        if (entry.is_regular_file())
            names.push_back(entry.path().filename().generic_string());

    // The order the OS returns entries in is unspecified, so sort before you
    // print or the output is not reproducible.
    std::sort(names.begin(), names.end());
    for (const std::string& n : names)
        std::cout << n << '\n';

    fs::remove_all(root, ec);
    return 0;
}
