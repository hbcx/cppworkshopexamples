#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fslink";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    fs::path target = root / "real.txt";
    std::ofstream(target) << "payload";

    // Creating (and following) a symlink needs privilege or OS support that is
    // not present everywhere -- notably Windows without Developer Mode -- so we
    // guard the whole sequence and fall back cleanly instead of crashing. The
    // point is the API: create_symlink makes the link, read_symlink reads its
    // target, is_symlink detects it, and symlink_status inspects the link ITSELF
    // while status() follows it to the target.
    fs::path link = root / "link.txt";
    try {
        fs::create_symlink(target, link);
        std::cout << "is_symlink(link):       " << fs::is_symlink(link) << '\n';
        std::cout << "read_symlink target:    "
                  << fs::read_symlink(link).filename().generic_string() << '\n';
        // status() follows the link -> regular file; symlink_status() does not.
        std::cout << "status follows to file: " << fs::is_regular_file(fs::status(link)) << '\n';
        std::cout << "symlink_status is link: " << fs::is_symlink(fs::symlink_status(link)) << '\n';
    } catch (const fs::filesystem_error&) {
        std::cout << "symlinks not available in this environment\n";
    }

    fs::remove_all(root, ec);
    return 0;
}
