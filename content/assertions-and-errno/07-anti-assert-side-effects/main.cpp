// Anti-pattern: putting a side effect inside assert(). assert compiles to nothing
// when NDEBUG is defined (release builds), so any work done inside the expression
// SILENTLY DISAPPEARS in release -- the program passes its tests in debug and
// misbehaves in production. Do the work first, then assert the result.
#define NDEBUG                 // simulate a release build so the trap is visible
#include <cassert>
#include <iostream>

int main() {
    std::cout << std::boolalpha;

    // WRONG: the increment is a SIDE EFFECT inside the assert. Under NDEBUG the
    // whole assert is removed, so ++processed never runs.
    int processed = 0;
    assert((++processed, processed == 1));
    std::cout << "wrong: processed under NDEBUG = " << processed << '\n';   // 0

    // RIGHT: do the work as normal code, then assert only inspects the result.
    int done = 0;
    ++done;                                // always runs
    assert(done == 1);                     // pure check, safe to compile out
    std::cout << "right: done = " << done << '\n';                          // 1
    return 0;
}
