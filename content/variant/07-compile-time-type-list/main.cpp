#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>    // std::index_sequence, std::make_index_sequence
#include <variant>

// Each file is its OWN type, carrying its metadata as constexpr static members.
// Nothing is stored at runtime -- the data lives in the type system.
struct ConfigFile {
    static constexpr std::string_view name = "config.ini";
    static constexpr std::string_view path = "/etc/app/config.ini";
    static constexpr std::size_t size = 512;
    static constexpr bool readOnly = true;
};
struct ReadmeFile {
    static constexpr std::string_view name = "README.md";
    static constexpr std::string_view path = "/opt/app/README.md";
    static constexpr std::size_t size = 2048;
    static constexpr bool readOnly = false;
};
struct DataFile {
    static constexpr std::string_view name = "data.bin";
    static constexpr std::string_view path = "/var/app/data.bin";
    static constexpr std::size_t size = 1048576;
    static constexpr bool readOnly = false;
};

// A std::variant used NOT to hold a runtime value, but as a compile-time LIST of
// types. Its traits -- variant_size and variant_alternative_t -- make it a table
// the compiler can query.
using Files = std::variant<ConfigFile, ReadmeFile, DataFile>;

// An enum names the positions -- the "keys" of the compile-time table.
enum FileId : std::size_t { Config = 0, Readme = 1, Data = 2 };

// variant_alternative_t<I, Files> is the type at position I, so an enum value
// selects a file type and exposes its constexpr members.
template <FileId Id>
using FileAt = std::variant_alternative_t<Id, Files>;

// A compile-time reduction over the whole list: sum every file's size with a
// fold, all resolved before the program runs.
template <std::size_t... Is>
constexpr std::size_t sumSizes(std::index_sequence<Is...>) {
    return (std::variant_alternative_t<Is, Files>::size + ...);
}
inline constexpr std::size_t kTotalSize =
    sumSizes(std::make_index_sequence<std::variant_size_v<Files>>{});

int main() {
    std::cout << std::boolalpha;

    // Every lookup here is a COMPILE-TIME constant -- no runtime map, no search.
    static_assert(FileAt<Config>::size == 512, "config size known at compile time");
    static_assert(FileAt<Config>::readOnly, "config is read-only");
    static_assert(!FileAt<Data>::readOnly, "data is writable");
    static_assert(std::variant_size_v<Files> == 3, "three files");
    static_assert(kTotalSize == 512 + 2048 + 1048576, "total computed at compile time");

    std::cout << "entries    = " << std::variant_size_v<Files> << '\n';
    std::cout << "total size = " << kTotalSize << '\n';
    std::cout << FileAt<Config>::name << "  " << FileAt<Config>::size
              << " bytes, ro=" << FileAt<Config>::readOnly << '\n';
    std::cout << FileAt<Readme>::name << "   " << FileAt<Readme>::size
              << " bytes, ro=" << FileAt<Readme>::readOnly << '\n';
    std::cout << FileAt<Data>::name << "    " << FileAt<Data>::size
              << " bytes, ro=" << FileAt<Data>::readOnly << '\n';
    return 0;
}
