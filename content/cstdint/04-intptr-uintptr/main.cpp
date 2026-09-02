#include <cstdint>
#include <iostream>

// uintptr_t / intptr_t are integers wide enough to hold a void* and convert
// back to the same pointer. They exist for the rare-but-real need to treat an
// address AS a number: a tagged pointer, an address-keyed table, an alignment
// check. Raw addresses vary from run to run, so this example prints only stable
// facts about the round-trip, never the address itself.
int main() {
    int value = 7;
    int* p = &value;

    std::uintptr_t asInt = reinterpret_cast<std::uintptr_t>(p);
    int* back = reinterpret_cast<int*>(asInt);

    std::cout << "round-trip gives back the same pointer : "
              << (back == p) << "\n";               // 1
    std::cout << "and it still points at the value       : "
              << (*back == 7) << "\n";              // 1

    std::cout << "\nuintptr_t is exactly as wide as a pointer:\n";
    std::cout << "sizeof(void*)     = " << sizeof(void*) << "\n";
    std::cout << "sizeof(uintptr_t) = " << sizeof(std::uintptr_t) << "\n";
    std::cout << "same width        = "
              << (sizeof(void*) == sizeof(std::uintptr_t)) << "\n";

    // A number lets you do bit tricks a pointer cannot -- for example test
    // alignment, the low bits of an address.
    bool aligned = (asInt % alignof(int)) == 0;
    std::cout << "\naddress is int-aligned : " << aligned << "\n";   // 1
}
