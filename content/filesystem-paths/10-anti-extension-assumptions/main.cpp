#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static void report(const fs::path& p) {
    std::cout << p.generic_string()
              << "  stem=" << p.stem().generic_string()
              << "  ext=" << p.extension().generic_string() << '\n';
}

int main() {
    // Anti-pattern: assuming extension() is "the text after the dot" without the
    // dot, and that every filename has one. The real rules surprise people:

    report("archive.tar.gz");   // ext=.gz  (only the LAST dot), stem=archive.tar
    report("Makefile");         // ext=""   (no dot at all)
    report(".gitignore");       // ext=""   -- a leading dot is NOT an extension
    report("photo.JPG");        // ext=.JPG (the dot IS included, case kept)

    // So: extension() KEEPS the leading dot, uses only the last dot, treats a
    // leading-dot name as all-stem, and is empty when there is no dot. Compare
    // extensions case-insensitively yourself if you need to, and never assume a
    // file has an extension.
    return 0;
}
