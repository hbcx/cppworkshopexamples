#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fscr";
    std::error_code ec;
    fs::remove_all(root, ec);

    // create_directory makes ONE level and fails if the parent is missing;
    // create_directories makes the whole chain. Prefer the plural for a nested
    // path.
    bool made = fs::create_directories(root / "a" / "b" / "c");
    std::cout << "created nested: " << made << '\n';                       // 1
    std::cout << "exists a/b/c:   " << fs::exists(root / "a" / "b" / "c") << '\n'; // 1

    // create_directories is idempotent: asking again returns false (nothing new
    // created) rather than failing.
    std::cout << "second call:    " << fs::create_directories(root / "a" / "b" / "c") << '\n'; // 0

    std::ofstream(root / "a" / "file.txt") << "x";

    // remove deletes a single file or an EMPTY directory; remove_all deletes a
    // whole tree and returns how many entries it removed.
    std::cout << "remove one file:  " << fs::remove(root / "a" / "file.txt") << '\n'; // 1
    std::uintmax_t n = fs::remove_all(root);
    std::cout << "remove_all count: " << n << '\n';   // root + a + b + c = 4

    fs::remove_all(root, ec);
    return 0;
}
