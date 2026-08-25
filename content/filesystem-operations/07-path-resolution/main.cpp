#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path root = fs::temp_directory_path() / "cppws_fsres";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "app");
    std::ofstream(root / "app" / "config.ini") << "x";

    // absolute() turns a relative path into an absolute one by joining it onto
    // the current directory. It is PURELY lexical -- it does not require the
    // file to exist and does not resolve '..' or symlinks.
    fs::path abs = fs::absolute("app/config.ini");
    std::cout << "absolute is_absolute: " << abs.is_absolute() << '\n';   // 1

    // canonical() returns the real, resolved path: it collapses '..', follows
    // symlinks, and REQUIRES the file to exist (it throws otherwise).
    fs::path real = fs::canonical(root / "app" / ".." / "app" / "config.ini");
    std::cout << "canonical filename: " << real.filename().generic_string() << '\n'; // config.ini
    std::cout << "no dotdot left: "
              << (real.generic_string().find("..") == std::string::npos) << '\n'; // 1

    // weakly_canonical() is canonical() for a path whose tail does not exist yet
    // -- perfect for an output file you are about to create.
    fs::path out = fs::weakly_canonical(root / "app" / "out" / "result.txt");
    std::cout << "weakly is_absolute: " << out.is_absolute() << '\n';   // 1

    // relative() expresses one path against a base (this one IS on disk).
    fs::path rel = fs::relative(root / "app" / "config.ini", root);
    std::cout << "relative: " << rel.generic_string() << '\n';   // app/config.ini

    fs::remove_all(root, ec);
    return 0;
}
