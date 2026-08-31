// Anti-pattern: a stateful allocator whose operator== always returns true.
//
// operator== means "these two can free each other's memory". For a stateful
// arena allocator that is only true when they share an arena. Claiming any two
// are equal lets a container move/swap a buffer between arenas that cannot free
// each other's memory.

#include <iostream>

// A tiny stand-in for a stateful allocator: an id naming its backing arena.
struct Alloc {
    int arenaId;
};

// BAD: ignores state, always equal.
bool wrong_equal(const Alloc&, const Alloc&) { return true; }

// GOOD: equal only when the arena matches.
bool correct_equal(const Alloc& a, const Alloc& b) { return a.arenaId == b.arenaId; }

int main() {
    Alloc a{1};   // backed by arena 1
    Alloc b{2};   // backed by arena 2 -- cannot free arena 1's memory

    std::cout << std::boolalpha;
    std::cout << "wrong operator==   says arenas 1 and 2 are equal: "
              << wrong_equal(a, b) << '\n';
    std::cout << "correct operator== says arenas 1 and 2 are equal: "
              << correct_equal(a, b) << '\n';

    // With the wrong operator==, a container move from b to a would let a's
    // allocator deallocate a pointer that came from arena 2 -- undefined. The
    // correct comparison forces an element-by-element move instead, which is
    // safe. (The cross-arena free is described, not run.)
    std::cout << "correct == tells them apart, so no cross-arena free happens\n";
}
