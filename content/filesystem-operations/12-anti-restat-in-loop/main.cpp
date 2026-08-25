#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: re-querying the disk with the free functions inside an
    // iteration loop -- fs::is_regular_file(e.path()), fs::file_size(e.path()) --
    // when the directory_entry you are holding already cached that information
    // while listing the directory. Each free-function call is another stat.
    fs::path root = fs::temp_directory_path() / "cppws_fscache";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    fs::path f = root / "log.txt";
    std::ofstream(f) << "12345";   // 5 bytes

    for (const fs::directory_entry& e : fs::directory_iterator(root)) {
        // Prefer the entry's own accessors: they answer from what the listing
        // already gathered, instead of a fresh fs::is_regular_file(e.path())
        // stat per entry.
        std::cout << "cached is_regular: " << e.is_regular_file() << '\n';  // 1
        std::cout << "cached size:       " << e.file_size() << '\n';        // 5
    }

    // The entry holds a SNAPSHOT taken when it was created. If the file changes
    // afterwards, call refresh() so the entry re-reads the current metadata (the
    // disk-reading free functions always see the current state). How much an
    // entry caches is implementation-defined, so refresh() after a change you
    // care about rather than relying on a value being either fresh or frozen.
    fs::directory_entry e(f);
    std::ofstream(f, std::ios::app) << "6789";   // grow to 9 bytes
    e.refresh();                                 // re-sync with the disk
    std::cout << "after refresh: " << e.file_size() << '\n';   // 9

    fs::remove_all(root, ec);
    return 0;
}
