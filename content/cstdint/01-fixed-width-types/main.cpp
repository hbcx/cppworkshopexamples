#include <cstdint>
#include <iostream>
#include <limits>

// The <cstdint> fixed-width types spell out how many bits an integer has, so a
// variable's size is the same on every platform. Plain int, long and so on are
// only guaranteed a MINIMUM width and differ between compilers and operating
// systems -- int32_t is always exactly 32 bits (or it does not exist at all).
int main() {
    std::int8_t   a = 0;
    std::int16_t  b = 0;
    std::int32_t  c = 0;
    std::int64_t  d = 0;
    std::uint32_t e = 0;
    (void)a; (void)b; (void)c; (void)d; (void)e;

    std::cout << "fixed-width sizes are the same everywhere:\n";
    std::cout << "sizeof int8_t   = " << sizeof(std::int8_t)   << " byte\n";
    std::cout << "sizeof int16_t  = " << sizeof(std::int16_t)  << " bytes\n";
    std::cout << "sizeof int32_t  = " << sizeof(std::int32_t)  << " bytes\n";
    std::cout << "sizeof int64_t  = " << sizeof(std::int64_t)  << " bytes\n";

    // Contrast: the built-in types only promise a minimum width, so these
    // numbers can differ on another platform.
    std::cout << "\nbuilt-in widths are platform-dependent:\n";
    std::cout << "sizeof int   = " << sizeof(int)  << " bytes\n";
    std::cout << "sizeof long  = " << sizeof(long) << " bytes (32 or 64 bits!)\n";

    std::cout << "\nranges are pinned down too:\n";
    std::cout << "int32_t  max = " << std::numeric_limits<std::int32_t>::max()  << "\n";
    std::cout << "uint32_t max = " << std::numeric_limits<std::uint32_t>::max() << "\n";
    std::cout << "int64_t  max = " << std::numeric_limits<std::int64_t>::max()  << "\n";
}
