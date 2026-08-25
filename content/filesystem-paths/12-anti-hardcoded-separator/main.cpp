#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: writing separators into a string literal, e.g.
    //     std::string bad = "logs\\2026\\app.log";   // backslash baked in
    // On Windows the OS accepts it, but on Linux and macOS the backslash is an
    // ordinary FILENAME character, not a separator -- so the whole thing becomes
    // one weird filename "logs\2026\app.log" with no directories at all. The bug
    // only shows up on the other platform, which is the worst kind.
    //
    // (We do not print that broken parse here because its result differs by
    // platform -- which is exactly the portability problem.)

    // Fix: never hardcode a separator. Build from components with operator/ and
    // let the library pick the right one; print with generic_string().
    fs::path good = fs::path("logs") / "2026" / "app.log";
    std::cout << "path:     " << good.generic_string() << '\n';             // logs/2026/app.log
    std::cout << "filename: " << good.filename().generic_string() << '\n'; // app.log
    std::cout << "parent:   " << good.parent_path().generic_string() << '\n'; // logs/2026
    return 0;
}
