// errno (from <cerrno>) is a thread-local integer that some C library functions
// set on failure. Two rules make it usable: it is NOT reset on success, so clear
// it to 0 before the call; and many functions signal failure another way too, so
// check that first and use errno to learn WHY. std::strtol is the classic case.
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <climits>

int main() {
    std::cout << std::boolalpha;

    // A value far beyond long: strtol clamps and sets errno to ERANGE.
    const char* text = "999999999999999999999999";
    errno = 0;                              // 1) reset before the call
    char* end = nullptr;
    long v = std::strtol(text, &end, 10);

    std::cout << "overflow (ERANGE): " << (errno == ERANGE) << '\n';  // true
    std::cout << "clamped to LONG_MAX: " << (v == LONG_MAX) << '\n';  // true

    // A clean parse: reset errno, and note strtol reports "nothing parsed" by
    // leaving end == start, not through errno.
    errno = 0;
    const char* good = "42";
    v = std::strtol(good, &end, 10);
    std::cout << "parsed 42: " << (v == 42 && errno == 0) << '\n';    // true
    std::cout << "consumed all input: " << (*end == '\0') << '\n';    // true
    return 0;
}
