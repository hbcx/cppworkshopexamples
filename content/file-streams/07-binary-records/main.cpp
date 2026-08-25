#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

struct Record {
    int id;
    double value;
};   // trivially copyable: no pointers, no owned resources

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_bin";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "records.dat";

    // Write two records as raw bytes, in binary mode.
    {
        std::ofstream out(file, std::ios::binary);
        Record recs[2] = { {1, 3.5}, {2, 7.25} };
        out.write(reinterpret_cast<const char*>(recs), sizeof recs);
    }

    // The file is exactly N * sizeof(Record) bytes -- fixed and seekable.
    std::cout << "file size: " << fs::file_size(file)
              << "  ==  2 * sizeof(Record): " << (2 * sizeof(Record)) << '\n';

    // Read the records back; the read() call is the loop condition.
    std::ifstream in(file, std::ios::binary);
    Record r{};
    while (in.read(reinterpret_cast<char*>(&r), sizeof r))
        std::cout << "id=" << r.id << " value=" << r.value << '\n';

    fs::remove_all(dir, ec);
    return 0;
}
