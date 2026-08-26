// static_assert is the compile-time cousin of assert: it checks a constant
// expression while COMPILING, and a false one is a compilation error with your
// message -- the program never builds, let alone runs. Use it for assumptions the
// compiler can verify: type requirements, type sizes, configuration.
#include <iostream>
#include <type_traits>
#include <cstdint>

template <typename T>
T narrow_cast(long long v) {
    // Refuse, at compile time, to instantiate this for a non-integer type.
    static_assert(std::is_integral<T>::value, "narrow_cast needs an integer type");
    return static_cast<T>(v);
}

// A layout assumption a serializer relies on: if it ever breaks, the build fails
// instead of writing corrupt bytes at run time.
static_assert(sizeof(std::int32_t) == 4, "expected a 4-byte int32");

int main() {
    std::cout << narrow_cast<int>(42) << '\n';
    std::cout << narrow_cast<short>(7) << '\n';

    // narrow_cast<double>(1) would FAIL to compile with the message above.
    static_assert(sizeof(char) == 1, "char is always 1 byte");   // always holds
    return 0;
}
