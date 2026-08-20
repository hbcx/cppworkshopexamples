#include <iostream>
#include <string>
#include <string_view>

int main() {
    using namespace std::string_view_literals;   // enables the sv suffix

    std::string owned = "from a std::string";
    std::string_view a = owned;                   // from std::string (implicit)

    std::string_view b = "from a literal";        // from const char* -- length up to the first NUL

    const char buf[] = {'a', 'b', 'c', 'd'};      // NOT null-terminated
    std::string_view c(buf, sizeof buf);          // from a (pointer, length) pair

    std::string_view d = "embedded\0nul"sv;       // the sv literal keeps the REAL length
    std::string_view e;                           // default: empty, data() may be null
    std::string_view f = "";                      // empty, but from a literal

    std::cout << "a = " << a << "  size " << a.size() << '\n';
    std::cout << "b = " << b << "  size " << b.size() << '\n';
    std::cout << "c = " << c << "  size " << c.size() << '\n';           // abcd, size 4
    std::cout << "d size = " << d.size()                                  // 12 (keeps the NUL)
              << ", but const char* stops at NUL: "
              << std::string_view("embedded\0nul").size() << '\n';        // 8
    std::cout << std::boolalpha;
    std::cout << "e.empty() = " << e.empty()
              << ", e.data() == nullptr : " << (e.data() == nullptr) << '\n';
    std::cout << "f.empty() = " << f.empty()
              << ", f.data() == nullptr : " << (f.data() == nullptr) << '\n';
    return 0;
}
