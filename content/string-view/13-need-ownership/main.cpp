#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

// A view is the right parameter when you only READ, and only DURING the call.
static std::size_t length(std::string_view sv) { return sv.size(); }

// WRONG shape (described): a view where the callee needs a C string. fopen wants
// a NUL-terminated const char*, which a view cannot promise:
//   FILE* openBad(std::string_view path) { return std::fopen(path.data(), "r"); }

// RIGHT: when you need NUL-termination (a C API) OR you keep the value, own it.
static void openConfig(const std::string& path) {
    if (FILE* f = std::fopen(path.c_str(), "r")) {
        std::fclose(f);
        std::cout << "opened " << path << '\n';
    } else {
        std::cout << "could not open " << path << '\n';
    }
}

// A SINK that KEEPS the text takes std::string by value and moves it into place.
struct Cache {
    std::string key;
    explicit Cache(std::string k) : key(std::move(k)) {}
};

int main() {
    std::cout << "length(\"borrow me\") = " << length("borrow me") << '\n';   // 9

    openConfig("definitely-missing.cfg");     // needs a real C string

    Cache c("kept forever");                  // stores -> must own
    std::cout << "cached key = " << c.key << '\n';

    // Rule of thumb:
    //   read, during the call     -> std::string_view
    //   read, but need a C string -> const std::string&  (NUL-terminated)
    //   store / keep the value    -> std::string (by value, then move)
    std::cout << "picked the owner wherever the value must outlive the call\n";
    return 0;
}
