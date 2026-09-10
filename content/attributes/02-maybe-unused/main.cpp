#include <iostream>

// This handler ignores its eventId. Without [[maybe_unused]] the compiler warns
// -Wunused-parameter, which is an error under -Werror.
int handle([[maybe_unused]] int eventId, int payload) {
    return payload * 2;
}

int main() {
    // A name that is used only in SOME builds -- here it would be read only by a
    // debug-only check that is compiled out in this build. Marking it says "yes,
    // it may be unused" so the compiler does not warn.
    [[maybe_unused]] const int retryLimit = 3;

    std::cout << "handle(7, 21) = " << handle(7, 21) << "\n";
    return 0;
}
