#include <iostream>

namespace mylib {
    namespace v1 {
        const char* greet() { return "hello (v1)"; }
    }
    // An inline namespace: its members also belong to the enclosing namespace, so
    // mylib::greet() resolves to v2::greet(). v2 is the default version; v1 is
    // still reachable by its explicit name for code that pinned it.
    inline namespace v2 {
        const char* greet() { return "hello (v2)"; }
    }
}

int main() {
    std::cout << "mylib::greet()     -> " << mylib::greet() << "\n";     // v2 (the inline one)
    std::cout << "mylib::v1::greet() -> " << mylib::v1::greet() << "\n"; // old version, still callable
    std::cout << "mylib::v2::greet() -> " << mylib::v2::greet() << "\n"; // named explicitly
    return 0;
}
