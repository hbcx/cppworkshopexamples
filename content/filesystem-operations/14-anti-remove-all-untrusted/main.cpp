#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// Is 'candidate', resolved under 'root', still contained by 'root'? Compare the
// lexically normalised forms so that '..' cannot climb out.
static bool inside(const fs::path& root, const fs::path& candidate) {
    fs::path r = root.lexically_normal();
    fs::path c = (root / candidate).lexically_normal();
    fs::path rel = c.lexically_relative(r);
    return !rel.empty() && *rel.begin() != "..";
}

int main() {
    // Anti-pattern: fs::remove_all on a path built from outside input without
    // checking it. remove_all deletes a whole tree and does not care what tree
    // -- an empty string, "/", or a "../.." climbing out of your workspace can
    // wipe far more than you meant. It is one of the sharpest edges in the API.
    fs::path root = fs::temp_directory_path() / "cppws_fssafe";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "keep");
    std::ofstream(root / "keep" / "important.txt") << "x";

    // Two "user supplied" targets to delete. One is fine, one escapes.
    fs::path ok  = "keep";
    fs::path bad = "../../etc";

    std::cout << "'keep' allowed:      " << inside(root, ok) << '\n';   // 1
    std::cout << "'../../etc' allowed: " << inside(root, bad) << '\n';  // 0

    // Only delete what passed the check. The escaping path is refused, never
    // handed to remove_all.
    if (inside(root, bad))
        fs::remove_all(root / bad);   // NOT reached
    else
        std::cout << "refused to delete outside the workspace\n";

    std::cout << "important.txt still here: "
              << fs::exists(root / "keep" / "important.txt") << '\n'; // 1

    fs::remove_all(root, ec);
    return 0;
}
