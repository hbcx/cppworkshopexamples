#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fsrec";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "src" / "gpu");
    fs::create_directories(root / "docs");
    std::ofstream(root / "README.md") << "r";
    std::ofstream(root / "src" / "main.cpp") << "m";
    std::ofstream(root / "src" / "gpu" / "shader.cpp") << "s";
    std::ofstream(root / "docs" / "guide.md") << "g";

    // recursive_directory_iterator descends into subdirectories. Its depth()
    // tells you how deep the current entry sits (it is a member of the ITERATOR,
    // not the entry). Report each file as depth + path relative to root, sorted
    // for a stable listing. lexically_relative is exact here because the
    // iterator builds its paths from root.
    std::vector<std::string> lines;
    for (fs::recursive_directory_iterator it(root), end; it != end; ++it) {
        if (!it->is_regular_file()) continue;
        fs::path rel = it->path().lexically_relative(root);
        lines.push_back(std::to_string(it.depth()) + " " + rel.generic_string());
    }
    std::sort(lines.begin(), lines.end());
    for (const std::string& l : lines) std::cout << l << '\n';

    // You can prune a branch: when you reach a directory you do not want to
    // descend into, call disable_recursion_pending() and the iterator skips it.
    int filesOutsideDocs = 0;
    for (fs::recursive_directory_iterator it(root), end; it != end; ++it) {
        if (it->is_directory() && it->path().filename() == "docs") {
            it.disable_recursion_pending();   // do not go into docs/
            continue;
        }
        if (it->is_regular_file()) ++filesOutsideDocs;
    }
    std::cout << "files seen when skipping docs/: " << filesOutsideDocs << '\n'; // 3

    fs::remove_all(root, ec);
    return 0;
}
