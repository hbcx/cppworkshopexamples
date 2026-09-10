#include <cstddef>
#include <cstring>   // std::memcpy
#include <iostream>
#include <string>

// ANTI-PATTERN (described): an extern "C" function taking or returning a
// non-trivial C++ type by value:
//
//   extern "C" std::string decorate(std::string s);   // C cannot build, copy, or
//                                                      // destroy this, and there is
//                                                      // no shared by-value ABI
//
// C has no notion of a constructor, destructor, or the string's layout, so this
// cannot be called from C and is not portable across the boundary.

// FIX: exchange C strings -- a const char* in, a caller-provided buffer out -- and
// let each side own its own memory. std::string is fine to use INSIDE.
extern "C" int decorate(const char* in, char* out, std::size_t outsize) {
    if (in == nullptr || out == nullptr)
        return 1;
    std::string tmp = std::string("[") + in + "]";   // C++ used internally, not at the edge
    if (tmp.size() + 1 > outsize)
        return 2;                                     // caller's buffer too small
    std::memcpy(out, tmp.c_str(), tmp.size() + 1);
    return 0;
}

int main() {
    char buf[32];
    if (decorate("hello", buf, sizeof(buf)) == 0)
        std::cout << "decorated = " << buf << "\n";
    return 0;
}
