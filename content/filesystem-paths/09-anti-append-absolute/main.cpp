#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: joining a second path that starts with a separator. If the
    // right-hand side is rooted, operator/ THROWS AWAY the left side and keeps
    // only the right -- a classic way to build a path that escapes the directory
    // you meant to stay inside.
    fs::path base = "srv/data/users";
    fs::path joined = base / "/etc/passwd";   // NOT srv/data/users/etc/passwd
    std::cout << "joined:   " << joined.generic_string() << '\n';  // /etc/passwd
    std::cout << "escaped:  " << (joined == fs::path("/etc/passwd")) << '\n';   // 1

    // Fix: append the RELATIVE part, so the join stays under base. Strip the
    // leading separator (here via relative_path()).
    fs::path tail = "/etc/passwd";
    fs::path safe = base / tail.relative_path();  // srv/data/users/etc/passwd
    std::cout << "safe:     " << safe.generic_string() << '\n';
    return 0;
}
