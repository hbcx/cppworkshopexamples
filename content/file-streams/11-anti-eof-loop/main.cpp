#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_eof";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "nums.txt";
    { std::ofstream(file) << "10 20 30\n"; }

    // Anti-pattern: while(!in.eof()). eofbit is set only AFTER a read goes past
    // the end, so the body runs one extra time on a failed read. The failed
    // extraction here (end of file reached while skipping whitespace) leaves x
    // holding its previous value, so the last number is processed a SECOND time.
    {
        std::ifstream in(file);
        std::cout << "wrong (eof in condition):\n";
        int x = 0;
        while (!in.eof()) {
            in >> x;
            std::cout << "  x = " << x << '\n';   // 10, 20, 30, 30  <- 30 twice
        }
    }

    // Fix: put the READ in the condition. The loop stops the moment a read fails.
    {
        std::ifstream in(file);
        std::cout << "right (read in condition):\n";
        int x = 0;
        while (in >> x)
            std::cout << "  x = " << x << '\n';   // 10, 20, 30
    }

    fs::remove_all(dir, ec);
    return 0;
}
