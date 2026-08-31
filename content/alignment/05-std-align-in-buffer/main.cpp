// std::align adjusts a pointer to the next boundary inside a buffer you already
// own. It updates the pointer and the remaining space, and returns null if the
// aligned block would not fit. This is the run-time tool a bump allocator uses.

#include <cstdint>
#include <iostream>
#include <memory>   // std::align

int main() {
    // A 16-byte-aligned buffer, so offsets are easy to reason about.
    alignas(16) unsigned char storage[64];

    // Start one byte in -- deliberately off the 16-byte boundary.
    void* ptr = storage + 1;
    std::size_t space = sizeof(storage) - 1;

    unsigned char* before = static_cast<unsigned char*>(ptr);
    void* result = std::align(16, sizeof(double), ptr, space);

    if (result == nullptr) {
        std::cout << "did not fit\n";
        return 0;
    }

    std::size_t advanced = static_cast<unsigned char*>(ptr) - before;
    bool aligned = reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;

    std::cout << "advanced " << advanced << " bytes to the next boundary\n";
    std::cout << "result is 16-aligned? " << (aligned ? "yes" : "no") << '\n';
    std::cout << "space left for the object: " << space << " bytes\n";
}
