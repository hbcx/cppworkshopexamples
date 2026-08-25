#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: turning ON symlink following in a recursive walk without
    // guarding against cycles. By DEFAULT recursive_directory_iterator does not
    // follow directory symlinks, which keeps it safe. If you pass
    // directory_options::follow_directory_symlink and the tree contains a link
    // that points back up into itself, the walk loops forever (or throws when it
    // runs out of handles).
    //
    //   // dangerous when 'root' contains a symlink cycle:
    //   for (auto& e : fs::recursive_directory_iterator(
    //            root, fs::directory_options::follow_directory_symlink)) { ... }
    //
    // We do not build a cycle here (creating symlinks is privileged on some
    // systems); the safe walk below uses the DEFAULT options.
    fs::path root = fs::temp_directory_path() / "cppws_fsfollow";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "a" / "b");
    std::ofstream(root / "a" / "x.txt") << "x";
    std::ofstream(root / "a" / "b" / "y.txt") << "y";

    // Safe: default options do not follow symlinks, so even a linked cycle could
    // not trap this loop. It simply visits the real tree once.
    int files = 0;
    for (const auto& e : fs::recursive_directory_iterator(root))
        if (e.is_regular_file()) ++files;
    std::cout << "files found (safe default walk): " << files << '\n';   // 2

    // If you MUST follow symlinks, cap the depth or remember the canonical()
    // paths you have already entered and stop when one repeats -- that breaks
    // the cycle.
    fs::remove_all(root, ec);
    return 0;
}
