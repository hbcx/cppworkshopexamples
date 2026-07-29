#include <cstring>
#include <iostream>
#include <string>

// A stand-in for a real C API: it takes a null-terminated string, nothing more.
static std::size_t c_api_length(const char* s) {
    return std::strlen(s);
}

int main() {
    std::string host = "example.com";

    // c_str() is guaranteed null-terminated -- safe to hand to a C function.
    std::cout << "c_api_length = " << c_api_length(host.c_str()) << '\n';
    std::cout << "size()       = " << host.size() << '\n';

    // The buffer is borrowed: valid only until the next modification.
    const char* p = host.c_str();
    std::cout << "before append: " << p << '\n';
    host += " (dangling now)";       // may reallocate -> p is invalidated here
    std::cout << "after append, re-fetch c_str(): " << host.c_str() << '\n';

    // Embedded null: std::string keeps its full length, but the C view stops
    // at the first null, so the two lengths disagree.
    std::string withNull("a\0b", 3);
    std::cout << "size() with embedded null = " << withNull.size() << '\n';
    std::cout << "strlen(c_str())           = " << c_api_length(withNull.c_str()) << '\n';
    return 0;
}
