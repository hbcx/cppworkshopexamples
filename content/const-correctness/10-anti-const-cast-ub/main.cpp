#include <iostream>
#include <string>
#include <cstddef>

// Anti-pattern: using const_cast to strip const and then WRITE through it. If
// the object was truly declared const, modifying it is undefined behaviour, even
// though the cast compiles. const_cast has exactly one safe use: to call a
// function that is not const-correct on an object that is NOT actually const.

// A legacy API that takes char* but does not modify it (it just forgot const):
std::size_t legacyLength(char* s) {
    std::size_t n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

int main() {
    // SAFE use: the object is not const. const_cast only removes a const on the
    // path to a non-const-correct API that we know does not write.
    std::string owned = "hello";
    const char* view = owned.c_str();
    std::size_t n = legacyLength(const_cast<char*>(view));  // OK: owned is mutable
    std::cout << "legacyLength = " << n << "\n";            // 5

    // UNDEFINED BEHAVIOUR (shown as a comment -- never run):
    //
    //     const int frozen = 10;
    //     int* p = const_cast<int*>(&frozen);
    //     *p = 20;                 // UB: writing to a truly const object
    //     std::cout << frozen;     // may print 10, 20, or crash
    //
    // The compiler may keep frozen in read-only memory or fold its value into
    // callers, so the write can be ignored or fault. Do not do this.

    // The real fix for the safe case is to correct the API (take const char*),
    // or to keep a mutable buffer if it genuinely needs writing.
    return 0;
}
