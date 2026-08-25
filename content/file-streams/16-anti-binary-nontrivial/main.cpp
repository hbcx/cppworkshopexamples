#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

namespace fs = std::filesystem;

struct BadRecord {
    int id;
    std::string name;   // owns heap data -- NOT trivially copyable
};

int main() {
    fs::path dir = fs::temp_directory_path() / "cppws_fs_serial";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::path file = dir / "rec.bin";

    // Anti-pattern: bit-blitting a pointer-owning type to disk. sizeof(BadRecord)
    // is CONSTANT no matter how long the name is -- the characters live on the
    // heap, not inside the object -- so write(&obj, sizeof obj) would store the
    // pointer, not the text. We do NOT write it that way.
    std::cout << "BadRecord trivially copyable: "
              << std::is_trivially_copyable<BadRecord>::value << '\n';   // 0
    BadRecord a{ 1, "x" };
    BadRecord b{ 1, "a very long name that lives on the heap" };
    std::cout << "same sizeof for short and long name: "
              << (sizeof a == sizeof b) << '\n';   // 1

    // Fix: serialize each field. Write the length, then the bytes.
    {
        std::ofstream out(file, std::ios::binary);
        std::string name = "Ada Lovelace";
        std::uint32_t len = static_cast<std::uint32_t>(name.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof len);
        out.write(name.data(), static_cast<std::streamsize>(name.size()));
    }

    // Read it back the same way: length first, then that many bytes.
    std::ifstream in(file, std::ios::binary);
    std::uint32_t len = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof len);
    std::string name(len, '\0');
    in.read(&name[0], static_cast<std::streamsize>(len));
    std::cout << "round-tripped name: " << name << " (len " << len << ")\n";

    fs::remove_all(dir, ec);
    return 0;
}
