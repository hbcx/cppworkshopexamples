#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fscp";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "src");
    std::ofstream(root / "src" / "a.txt") << "12345";   // 5 bytes
    std::ofstream(root / "src" / "b.txt") << "67";      // 2 bytes

    // copy_file copies one file. copy_options decides what happens if the target
    // exists: overwrite_existing replaces it, skip_existing leaves it.
    fs::copy_file(root / "src" / "a.txt", root / "copy.txt",
                  fs::copy_options::overwrite_existing);
    std::cout << "copy size: " << fs::file_size(root / "copy.txt") << '\n';  // 5

    // Copy a whole directory tree with the recursive option.
    fs::copy(root / "src", root / "backup", fs::copy_options::recursive);
    std::cout << "backup has a.txt: " << fs::exists(root / "backup" / "a.txt") << '\n'; // 1
    std::cout << "backup has b.txt: " << fs::exists(root / "backup" / "b.txt") << '\n'; // 1

    // rename MOVES a file (or renames it): the old name is gone, the new one is
    // there. It is the atomic way to publish a file you built under a temp name.
    fs::rename(root / "copy.txt", root / "final.txt");
    std::cout << "old gone:  " << !fs::exists(root / "copy.txt") << '\n';   // 1
    std::cout << "new there: " << fs::exists(root / "final.txt") << '\n';   // 1

    fs::remove_all(root, ec);
    return 0;
}
