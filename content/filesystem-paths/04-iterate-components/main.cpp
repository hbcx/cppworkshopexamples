#include <filesystem>
#include <iostream>
#include <iterator>

namespace fs = std::filesystem;

int main() {
    // A path is iterable: it yields its components in order. For an absolute
    // path the first component is the root. Iteration is how you walk or rebuild
    // a path piece by piece.
    fs::path p = "/etc/nginx/sites/default.conf";

    std::cout << "components:";
    for (const fs::path& part : p)
        std::cout << ' ' << part.generic_string();
    std::cout << '\n';

    // std::distance over the path counts its components (a standard algorithm
    // working on the path's own iterators).
    auto count = std::distance(p.begin(), p.end());
    std::cout << "count: " << count << '\n';

    // Rebuild the path from its parts with /= -- you get the original back.
    fs::path rebuilt;
    for (const fs::path& part : p)
        rebuilt /= part;
    std::cout << "rebuilt: " << rebuilt.generic_string() << '\n';
    std::cout << "matches: " << (rebuilt == p) << '\n';
    return 0;
}
