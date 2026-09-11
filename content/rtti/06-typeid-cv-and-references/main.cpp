#include <iostream>
#include <typeinfo>

struct Poly { virtual ~Poly() = default; };

// Hide the null from the compiler so it cannot warn about a static null deref --
// the point is the run-time std::bad_typeid, not a compile-time diagnostic.
Poly* getNull() { return nullptr; }

int main() {
    std::cout << std::boolalpha;

    // typeid ignores top-level cv-qualifiers and reference-ness.
    std::cout << "const int == int?  " << (typeid(const int) == typeid(int)) << "\n";
    std::cout << "int& == int?       " << (typeid(int&) == typeid(int)) << "\n";
    std::cout << "const int& == int? " << (typeid(const int&) == typeid(int)) << "\n";

    // typeid on a dereferenced NULL pointer to a POLYMORPHIC type throws bad_typeid.
    Poly* p = getNull();
    try {
        (void) typeid(*p);
        std::cout << "no throw (not reached)\n";
    } catch (const std::bad_typeid&) {
        std::cout << "typeid(*null polymorphic) threw std::bad_typeid\n";
    }
    return 0;
}
