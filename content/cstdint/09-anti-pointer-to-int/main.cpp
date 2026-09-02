#include <cstdint>
#include <iostream>

// TRAP: stashing a pointer in an int or a long. A pointer is 64 bits on a
// 64-bit system, but int is 32 bits everywhere and long is 32 bits on Windows.
// The high half of the address is silently chopped off, and converting back
// gives a different, wrong pointer. Rather than dereference a real truncated
// address (undefined behaviour), we show the loss on a value we control.
int main() {
    // Stand-in for a 64-bit address that has data in its high 32 bits.
    std::uintptr_t address = UINT64_C(0x1234567800000000) | 0xABCDu;

    // "Store it in 32 bits" -- what happens when a pointer meets int/long32.
    std::uint32_t truncated = static_cast<std::uint32_t>(address);

    std::cout << std::hex;
    std::cout << "original (uintptr_t)  : 0x" << address   << "\n";
    std::cout << "forced into 32 bits   : 0x" << truncated << "   <- high half is gone\n";
    std::cout << std::dec;
    std::cout << "round-trips to the same value : "
              << (static_cast<std::uintptr_t>(truncated) == address)
              << "   (0 = corrupted)\n";

    std::cout << "\nwhy it does not fit:\n";
    std::cout << "sizeof(void*) = " << sizeof(void*)
              << ", sizeof(long) = " << sizeof(long) << "\n";

    std::cout << "\nFIX: hold an address in uintptr_t / intptr_t, never int or long.\n";
}
