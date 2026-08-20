#include <iostream>
#include <string_view>

int main() {
    std::string_view path = "/usr/local/bin/clang";

    std::cout << "first '/' at " << path.find('/') << '\n';    // 0
    std::cout << "last  '/' at " << path.rfind('/') << '\n';    // right before "clang"

    std::size_t dot = path.find_first_of(".-");                 // none in this path
    std::cout << "first '.' or '-': "
              << (dot == std::string_view::npos
                      ? std::string_view("none")
                      : path.substr(dot, 1)) << '\n';

    // The last path segment, as a view -- no copy.
    std::string_view base = path.substr(path.rfind('/') + 1);
    std::cout << "basename = " << base << '\n';                 // clang

    // compare and == work directly on views, and mix with literals for free.
    std::cout << std::boolalpha;
    std::cout << "base == \"clang\" : " << (base == "clang") << '\n';   // true
    std::cout << "base <  \"gcc\"   : " << (base < "gcc") << '\n';      // true ('c' < 'g')

    // Manual prefix test -- starts_with/ends_with arrive in C++20 (see notes).
    std::string_view prefix = "/usr";
    bool underUsr = path.substr(0, prefix.size()) == prefix;
    std::cout << "under /usr : " << underUsr << '\n';           // true
    return 0;
}
