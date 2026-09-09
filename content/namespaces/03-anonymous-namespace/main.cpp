#include <iostream>
#include "parts.hpp"

// An UNNAMED namespace gives its members internal linkage: they are private to
// this translation unit. This is the modern replacement for file-scope `static`.
namespace {
    const char* who() { return "main.cpp"; }
}

std::string fromMain() { return who(); }   // uses THIS file's who()

int main() {
    // other.cpp has its OWN who() in its own unnamed namespace. The two never
    // clash at link time, because each is private to its translation unit.
    std::cout << "fromMain()  -> " << fromMain() << "\n";
    std::cout << "fromOther() -> " << fromOther() << "\n";
    return 0;
}
