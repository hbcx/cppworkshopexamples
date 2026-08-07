#include <iostream>
#include <type_traits>
#include <vector>

// BAD: someone added a move constructor for speed. Declaring ANY move operation
// makes the compiler DELETE the copy constructor and copy assignment, so the
// class is now silently move-only -- any code that copies it stops compiling.
struct AlmostMovable {
    std::vector<int> data;
    AlmostMovable() = default;
    AlmostMovable(AlmostMovable&&) noexcept = default;   // this deletes the copy
};

// GOOD: if you still want copies, bring them back with = default.
struct Movable {
    std::vector<int> data;
    Movable() = default;
    Movable(Movable&&) noexcept = default;
    Movable& operator=(Movable&&) noexcept = default;
    Movable(const Movable&) = default;                   // copy brought back
    Movable& operator=(const Movable&) = default;
};

int main() {
    std::cout << std::boolalpha;
    std::cout << "AlmostMovable copyable? "
              << std::is_copy_constructible<AlmostMovable>::value
              << "  <- false: the move ctor deleted the copy\n";
    std::cout << "Movable copyable?       "
              << std::is_copy_constructible<Movable>::value
              << "  <- true: = default brought it back\n";

    // The trap: `AlmostMovable b = a;` would not compile, even though the author
    // never meant to forbid copying -- declaring the move quietly deleted it.
    return 0;
}
