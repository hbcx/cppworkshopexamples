#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static void dump(const fs::path& file, const char* label) {
    std::ifstream in(file);
    std::string line;
    std::cout << label << ":\n";
    while (std::getline(in, line))
        std::cout << "  " << line << '\n';
}

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_modes";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "log.txt";

    // Default mode truncates: opening for write empties any existing file.
    { std::ofstream(file) << "first\n"; }
    { std::ofstream(file) << "second\n"; }   // "first" is gone
    dump(file, "after two default writes (truncate)");

    // Append mode keeps the existing content and writes at the end.
    { std::ofstream out(file, std::ios::app); out << "third\n"; }
    dump(file, "after an append write");

    // Other flags used elsewhere in the chapter:
    //   std::ios::binary  -- no line-ending translation (binary examples)
    //   std::ios::ate     -- open positioned at the end, but seeking is allowed
    //   std::ios::in | std::ios::out -- read and write one stream (fstream)

    fs::remove_all(dir, ec);
    return 0;
}
