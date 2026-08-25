#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // A path splits into named parts. These are all member queries and none of
    // them look at the disk -- they read the stored string.
    fs::path p = "project/src/main.cpp";
    std::cout << "path:        " << p.generic_string() << '\n';
    std::cout << "filename:    " << p.filename().generic_string() << '\n';    // main.cpp
    std::cout << "stem:        " << p.stem().generic_string() << '\n';        // main
    std::cout << "extension:   " << p.extension().generic_string() << '\n';   // .cpp
    std::cout << "parent_path: " << p.parent_path().generic_string() << '\n'; // project/src

    // The root part matters for absolute paths. A POSIX-style absolute path has
    // a root-directory "/" and, after it, the relative part. (On Windows an
    // absolute path also carries a root-name like "C:"; see the portability
    // example.)
    fs::path abs = "/usr/local/bin/clang";
    std::cout << "root_directory: " << abs.root_directory().generic_string() << '\n'; // /
    std::cout << "root_path:      " << abs.root_path().generic_string() << '\n';      // /
    std::cout << "relative_path:  " << abs.relative_path().generic_string() << '\n';  // usr/local/bin/clang

    std::cout << "has_extension:   " << p.has_extension() << '\n';
    std::cout << "has_parent_path: " << p.has_parent_path() << '\n';
    return 0;
}
