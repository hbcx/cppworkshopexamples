#include <any>
#include <iostream>
#include <typeinfo>

int main() {
    std::any a = 7;   // int

    // Value form: any_cast<T>(a) returns a copy, and THROWS std::bad_any_cast if
    // T is not exactly the stored type -- no conversions, not even int <-> long.
    std::cout << "value cast: " << std::any_cast<int>(a) << '\n';
    try {
        (void) std::any_cast<long>(a);   // stored int, asked long -> throws
    } catch (const std::bad_any_cast&) {
        std::cout << "any_cast<long> threw: stored type is int, not long\n";
    }

    // Pointer form: any_cast<T>(&a) returns T* on a match, or nullptr on a
    // mismatch -- a checked cast with no exception.
    if (int* p = std::any_cast<int>(&a)) {
        std::cout << "pointer cast ok: " << *p << '\n';
    }
    if (std::any_cast<double>(&a) == nullptr) {
        std::cout << "pointer cast to double: nullptr (wrong type)\n";
    }
    return 0;
}
