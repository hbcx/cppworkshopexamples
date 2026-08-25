#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static int countLines(const fs::path& file) {
    std::ifstream in(file);
    std::string s;
    int n = 0;
    while (std::getline(in, s)) ++n;
    return n;
}

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_endl";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path slow = dir / "slow.txt";
    fs::path fast = dir / "fast.txt";

    // Anti-pattern: std::endl at the end of every line. endl writes '\n' AND
    // flushes to the OS -- the flush happens 1000 times here.
    {
        std::ofstream out(slow);
        for (int i = 0; i < 1000; ++i)
            out << "line " << i << std::endl;
    }

    // Fix: write '\n' and let the stream buffer; it flushes once at close.
    {
        std::ofstream out(fast);
        for (int i = 0; i < 1000; ++i)
            out << "line " << i << '\n';
    }

    // Same content -- only the number of flushes differed.
    std::cout << "slow lines: " << countLines(slow) << '\n';   // 1000
    std::cout << "fast lines: " << countLines(fast) << '\n';   // 1000
    std::cout << "use '\\n' in loops; reserve endl/flush for when you truly need a flush\n";

    fs::remove_all(dir, ec);
    return 0;
}
