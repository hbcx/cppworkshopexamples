#include <iostream>
#include <cstdint>

int main() {
    int value = 0x01020304;

    // Defined use 1: pointer <-> integer round trip. The restored pointer equals
    // the original, so we can read the value back through it.
    int* p = &value;
    std::uintptr_t asInt = reinterpret_cast<std::uintptr_t>(p);
    int* restored = reinterpret_cast<int*>(asInt);
    std::cout << "pointer round-trips? " << (restored == p) << "\n";   // 1

    // Defined use 2: inspect an object's raw bytes through unsigned char*. Viewing
    // any object as bytes is explicitly allowed by the aliasing rules. We rebuild
    // the value from its bytes in the SAME order, so the result is endian-neutral.
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    unsigned int rebuilt = 0;
    for (std::size_t i = 0; i < sizeof(value); ++i)
        rebuilt |= static_cast<unsigned int>(bytes[i]) << (8 * i);
    std::cout << "rebuilt from its own bytes matches? "
              << (rebuilt == static_cast<unsigned int>(value)) << "\n";  // 1
    return 0;
}
