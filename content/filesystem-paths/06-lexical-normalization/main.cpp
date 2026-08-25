#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // The lexically_* members simplify a path by TEXT alone -- they never look
    // at the disk, so they work on paths that do not exist and never resolve a
    // symlink. (canonical(), in the operations chapter, is the disk-based
    // counterpart.)

    // lexically_normal collapses '.', '..' and doubled separators.
    fs::path messy = "a/./b/../c//d/";
    std::cout << "normal: " << messy.lexically_normal().generic_string() << '\n'; // a/c/d/

    // lexically_relative expresses one path relative to a base, again purely by
    // text.
    fs::path full = "project/src/gpu/shader.cpp";
    fs::path base = "project/src";
    std::cout << "relative: " << full.lexically_relative(base).generic_string() << '\n'; // gpu/shader.cpp

    // It can walk up with '..' when the target is outside the base.
    fs::path other = "project/docs/readme.md";
    std::cout << "up-and-over: " << other.lexically_relative(base).generic_string() << '\n'; // ../docs/readme.md
    return 0;
}
