#include <iostream>
#include <tuple>
#include <utility>

// A type that counts how it is built, so we can see moves appear or vanish.
// Counters are static, so the totals do not depend on argument evaluation
// order -- the output is identical on every compiler.
struct Loud {
    Loud(int, int) { ++ctors; }
    Loud(const Loud&) { ++copies; }
    Loud(Loud&&) noexcept { ++moves; }
    static int ctors;
    static int copies;
    static int moves;
};
int Loud::ctors = 0;
int Loud::copies = 0;
int Loud::moves = 0;

int main() {
    // Normal construction: each Loud temporary is built (ctors), then moved
    // into the pair member (moves).
    {
        std::pair<Loud, Loud> normal(Loud(1, 2), Loud(3, 4));
        (void)normal;
    }
    std::cout << "normal:    ctors=" << Loud::ctors
              << " moves=" << Loud::moves
              << " copies=" << Loud::copies << '\n';

    Loud::ctors = 0;
    Loud::moves = 0;
    Loud::copies = 0;

    // piecewise_construct forwards the ARGUMENTS as tuples, so each member is
    // built in place: the ctors still happen, but no move and no copy.
    {
        std::pair<Loud, Loud> piece(
            std::piecewise_construct,
            std::forward_as_tuple(1, 2),
            std::forward_as_tuple(3, 4));
        (void)piece;
    }
    std::cout << "piecewise: ctors=" << Loud::ctors
              << " moves=" << Loud::moves
              << " copies=" << Loud::copies << '\n';
    return 0;
}
