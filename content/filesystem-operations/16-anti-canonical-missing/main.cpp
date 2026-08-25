#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: calling canonical() on a path that does not exist yet -- for
    // example the OUTPUT file you are about to write. canonical() resolves
    // against the real filesystem and THROWS if the path is not there, so this
    // crashes exactly when you build a fresh output path.
    fs::path root = fs::temp_directory_path() / "cppws_fscanon";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root);
    fs::path notYet = root / "build" / "out.o";   // does not exist

    try {
        fs::path p = fs::canonical(notYet);
        std::cout << "unexpected: " << p.generic_string() << '\n';
    } catch (const fs::filesystem_error&) {
        std::cout << "canonical threw on the missing path\n";
    }

    // Fix: weakly_canonical() resolves the part that exists and keeps the rest
    // lexically -- it never requires the tail to be there, so it is the right
    // tool for a not-yet-created path.
    fs::path resolved = fs::weakly_canonical(notYet);
    std::cout << "weakly_canonical is_absolute: " << resolved.is_absolute() << '\n'; // 1
    std::cout << "ends with out.o: " << (resolved.filename() == "out.o") << '\n';    // 1

    // Or, if you only need it absolute (no symlink/.. resolution), absolute()
    // is purely lexical and also never touches the missing file.
    std::cout << "absolute is_absolute: " << fs::absolute(notYet).is_absolute() << '\n'; // 1

    fs::remove_all(root, ec);
    return 0;
}
