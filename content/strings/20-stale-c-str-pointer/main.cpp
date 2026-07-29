#include <cstring>
#include <iostream>
#include <string>

// A stand-in C API that only reads a null-terminated string right now.
static std::size_t c_api_length(const char* s) { return std::strlen(s); }

int main() {
    std::string s = "short";

    // WRONG (described, not run): cache the pointer, then grow the string. The
    // append can reallocate, freeing the block `stale` points at; using it after
    // is undefined behaviour even though it often "works" until it doesn't.
    //   const char* stale = s.c_str();
    //   s += " and now much, much longer than the small-string buffer allows";
    //   c_api_length(stale);          // reads freed memory -- UB

    // RIGHT: mutate first, then fetch c_str(), and never hold it across a change.
    s += " and now much, much longer than the small-string buffer allows";
    const char* fresh = s.c_str();     // valid: nothing mutates s after this
    std::cout << "length via C API: " << c_api_length(fresh) << '\n';
    std::cout << "matches size():   " << (c_api_length(fresh) == s.size() ? "yes" : "no") << '\n';

    // RIGHT: if the C side must KEEP the pointer, hand it a copy it owns.
    std::string owned = s;             // an independent buffer for the C side
    const char* stable = owned.c_str();
    s += " (this later change cannot disturb the copy)";
    std::cout << "copy still intact: " << (c_api_length(stable) == owned.size() ? "yes" : "no") << '\n';
    return 0;
}
