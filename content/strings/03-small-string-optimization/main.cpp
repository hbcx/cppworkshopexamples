#include <iostream>
#include <string>

// Is the string's character buffer stored INSIDE the object itself (SSO) or on
// the heap? We answer with a pointer comparison -- never a raw capacity number,
// whose exact value is implementation-defined and would differ across libraries.
static bool storesInline(const std::string& s) {
    const char* buf = s.data();
    const char* self = reinterpret_cast<const char*>(&s);
    return buf >= self && buf < self + sizeof(s);
}

int main() {
    std::string shortStr = "hi";
    std::string longStr  = "this string is far too long to fit in the inline buffer";

    std::cout << "sizeof(std::string) = " << sizeof(std::string) << " bytes\n";
    std::cout << "short (\"" << shortStr << "\") stored inline? "
              << (storesInline(shortStr) ? "yes -- no heap" : "no") << '\n';
    std::cout << "long stored inline?  "
              << (storesInline(longStr) ? "yes" : "no -- on the heap") << '\n';

    // A freshly constructed empty string already has non-zero capacity: that is
    // the inline buffer standing ready, not a heap allocation.
    std::string empty;
    std::cout << "empty string capacity is non-zero? "
              << (empty.capacity() > 0 ? "yes" : "no") << '\n';
    std::cout << "empty string inline? " << (storesInline(empty) ? "yes" : "no") << '\n';
    return 0;
}
