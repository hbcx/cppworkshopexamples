// Anti-pattern: forwarding (or moving) the same argument more than once. After
// std::forward<T>(x) passes an rvalue to a sink that moves from it, x is
// moved-from; forwarding it AGAIN moves from an already-gutted object. Each
// forwarding-reference argument should be forwarded exactly once, on its last use.
#include <iostream>
#include <string>
#include <utility>

void use(std::string s) { std::cout << "  use got length " << s.size() << '\n'; }

// WRONG: forwards x twice. With an rvalue caller the second forward moves from an
// already-moved-from string. (Instantiated below only with an lvalue, where
// forward makes no move, so it compiles cleanly.)
template <typename T>
void two_wrong(T&& x) {
    use(std::forward<T>(x));       // may move x
    use(std::forward<T>(x));       // x may already be gutted here
}

// RIGHT: read through a non-consuming reference for the earlier use, forward last.
template <typename T>
void two_right(T&& x) {
    const std::string& ref = x;    // read without consuming
    use(ref);                      // safe: a copy, no move
    use(std::forward<T>(x));        // forward exactly once, on the final use
}

int main() {
    std::cout << "two_right with an rvalue (forward once, on the last use):\n";
    two_right(std::string("hello"));      // both uses see length 5

    std::cout << "two_wrong with an lvalue (safe: forward yields an lvalue):\n";
    std::string lv = "data";
    two_wrong(lv);                        // both uses copy; an RVALUE would double-move
    return 0;
}
