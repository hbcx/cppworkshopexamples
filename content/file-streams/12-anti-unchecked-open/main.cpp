#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_open";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    // Anti-pattern: not checking whether the file opened. The parent directory
    // does not exist, so the open fails -- but nothing throws, and the write is
    // a silent no-op.
    fs::path bad = dir / "no_such_subdir" / "out.txt";
    std::ofstream out(bad);
    out << "important data\n";        // goes nowhere
    std::cout << "stream ok after write: " << static_cast<bool>(out) << '\n'; // 0
    std::cout << "file created: " << fs::exists(bad) << '\n';                 // 0

    // Fix: check the stream right after opening and handle the failure.
    fs::path good = dir / "out.txt";
    std::ofstream out2(good);
    if (!out2) {
        std::cout << "open failed\n";
        return 1;
    }
    out2 << "important data\n";
    out2.close();
    std::cout << "written: " << fs::exists(good) << '\n';   // 1

    fs::remove_all(dir, ec);
    return 0;
}
