#include <cstdint>
#include <iostream>
#include <limits>

// TRAP: reaching for int32_t out of habit when the code only needs "an integer
// that can hold at least about two billion". The exact-width types are OPTIONAL:
// the standard guarantees intN_t only on platforms that have a two's-complement
// type of exactly N bits with no padding. That covers every mainstream desktop
// and server, but some DSPs and embedded targets (9-bit bytes, padded words)
// provide no int32_t at all, and code that names it will not compile there.
// int_least32_t and int_fast32_t are ALWAYS provided.
int main() {
    std::int_least32_t counter = 0;   // portable "at least 32 bits"
    std::int32_t       field   = 0;   // exact -- reserve it for fixed layouts
    (void)counter; (void)field;

    std::cout << "int_least32_t : always available, at least "
              << std::numeric_limits<std::int_least32_t>::digits << " value bits\n";
    std::cout << "int32_t       : optional, exactly 32 bits where it exists\n";

    std::cout << "\nchoose by intent:\n";
    std::cout << "  need a range, portable code     -> int_leastN_t / int_fastN_t\n";
    std::cout << "  need an exact on-disk/wire size  -> intN_t\n";

    std::cout << "\n(on this platform int32_t exists, so both compile fine --\n"
                 " the difference bites only on unusual hardware.)\n";
}
