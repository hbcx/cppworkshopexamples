#include <cstdint>
#include <cinttypes>   // PRId64, PRIu32, ... printf format-string macros
#include <cstdio>

// printf cannot see the type you pass -- you name it in the format string, and
// "%d" means int. There is no portable spelling for int64_t, because it may be
// long on one platform and long long on another. <cinttypes> fixes this with
// macros that expand to the correct length modifier: PRId64 for int64_t,
// PRIu32 for uint32_t, and so on. (With C++ streams none of this is needed --
// << already knows the type -- but printf and C-interop code do.)
int main() {
    std::int64_t  big   = 9000000000;         // does not fit in 32 bits
    std::uint32_t count = 4000000000u;
    std::int32_t  small = -12345;

    std::printf("int64_t  via PRId64 : %" PRId64 "\n", big);
    std::printf("uint32_t via PRIu32 : %" PRIu32 "\n", count);
    std::printf("int32_t  via PRId32 : %" PRId32 "\n", small);

    // Hex and the other bases have matching macros.
    std::printf("uint32_t via PRIx32 : %" PRIx32 "\n", count);
    return 0;
}
