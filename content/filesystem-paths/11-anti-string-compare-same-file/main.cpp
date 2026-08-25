#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: deciding two paths point at the same file by comparing their
    // text. Equality (and string compare) is purely lexical -- these three spell
    // the same location but are all unequal:
    fs::path a = "data/reports/q1.csv";
    fs::path b = "./data/reports/q1.csv";
    fs::path c = "data/reports/../reports/q1.csv";

    std::cout << "a == b: " << (a == b) << '\n';   // 0
    std::cout << "a == c: " << (a == c) << '\n';   // 0

    // Fix (lexical): normalise both first, then they match by text.
    std::cout << "normalised a==b: "
              << (a.lexically_normal() == b.lexically_normal()) << '\n';   // 1
    std::cout << "normalised a==c: "
              << (a.lexically_normal() == c.lexically_normal()) << '\n';   // 1

    // Fix (on disk): the only true "same file" test is fs::equivalent(a, b),
    // which resolves both against the real filesystem (it needs the files to
    // exist -- see the operations chapter). Text comparison cannot see symlinks
    // or two mounts of the same volume.
    return 0;
}
