// NDEBUG turns asserts OFF. Defining NDEBUG before including <cassert> (release
// builds do this, e.g. -DNDEBUG) makes every assert expand to nothing -- the
// condition is not even evaluated. This example defines it to show the release
// behaviour: a false assert does not stop the program.
#define NDEBUG
#include <cassert>
#include <iostream>

int main() {
    std::cout << std::boolalpha;

    // With NDEBUG, this false assert is a no-op -- no abort, execution continues.
    assert(false && "would fire in a debug build");
    std::cout << "reached code after a false assert (NDEBUG on)\n";

    // Because the condition is not evaluated under NDEBUG, any SIDE EFFECT inside
    // an assert disappears in release. Here the counter is never incremented.
    int calls = 0;
    assert(++calls > 0);
    std::cout << "calls after assert under NDEBUG: " << calls << '\n';   // 0

    // Lesson: asserts are debug-only. Keep checks that must always happen --
    // validating input, checking I/O results -- as real code: if/throw or error_code.
    return 0;
}
