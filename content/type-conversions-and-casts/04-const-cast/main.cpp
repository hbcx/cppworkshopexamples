#include <iostream>
#include <cstddef>

// A legacy-style API that takes char* but only READS it -- it should have taken
// const char*, but we cannot change it. const_cast lets us call it with const data
// we know it will not modify.
std::size_t legacyLength(char* s) {
    std::size_t n = 0;
    while (s[n] != '\0') ++n;
    return n;
}

int main() {
    const char* message = "hello, world";

    // Strip const to satisfy the API's signature. Safe here: legacyLength only
    // reads, so nothing is written through the cast-away pointer.
    std::size_t len = legacyLength(const_cast<char*>(message));
    std::cout << "length via legacy API: " << len << "\n";   // 12

    // Note: this is only sound because legacyLength does not modify. Using
    // const_cast to actually WRITE to a truly const object is undefined behavior
    // -- see the const_cast anti-pattern.
    return 0;
}
