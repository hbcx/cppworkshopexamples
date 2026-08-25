#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Build paths from components with operator/ and print them with
    // generic_string(): the result uses '/' on every platform, so output,
    // config files and anything you check into git look the same on Windows,
    // Linux and macOS.
    fs::path p = fs::path("assets") / "textures" / "wood.png";
    std::cout << "generic: " << p.generic_string() << '\n';   // assets/textures/wood.png

    // native()/string() use the platform's preferred separator instead -- '\\'
    // on Windows. That form is for handing to the OS, not for storing or
    // comparing across machines. The character the platform prefers is exposed
    // as path::preferred_separator, which is why the same code produces the
    // right separator everywhere without you choosing.

    // Equality is component-wise, so two spellings assembled differently but
    // naming the same location are equal on every platform.
    fs::path built = fs::path("a") / "b" / "c";
    std::cout << "equal: " << (built == fs::path("a/b/c")) << '\n';   // 1

    // A path also orders, so it works as a map key or in a sorted set -- useful
    // for de-duplicating a list of files.
    std::cout << "ordered: " << (fs::path("a/b") < fs::path("a/c")) << '\n';   // 1
    return 0;
}
