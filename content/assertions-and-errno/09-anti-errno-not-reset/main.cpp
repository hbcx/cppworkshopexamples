// Anti-pattern: reading errno without clearing it first. errno is set on failure
// but NOT reset on success, so a stale value from an earlier failed call is still
// there. Checking errno after a call that actually succeeded then reports a
// failure that did not happen. Set errno = 0 before the call, every time.
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <climits>

int main() {
    std::cout << std::boolalpha;

    // An earlier call fails and leaves errno = ERANGE.
    errno = 0;
    (void)std::strtol("999999999999999999999999", nullptr, 10);   // sets ERANGE

    // WRONG: a second, SUCCESSFUL parse, but we forgot to reset errno. The stale
    // ERANGE is still set, so this good parse looks like an overflow.
    long v = std::strtol("42", nullptr, 10);
    std::cout << "wrong: parsed " << v
              << " but errno says overflow = " << (errno == ERANGE) << '\n';   // true (stale!)

    // RIGHT: reset errno immediately before the call, then it reflects THIS call.
    errno = 0;
    v = std::strtol("42", nullptr, 10);
    std::cout << "right: parsed " << v
              << " and errno clean = " << (errno == 0) << '\n';                 // true
    return 0;
}
