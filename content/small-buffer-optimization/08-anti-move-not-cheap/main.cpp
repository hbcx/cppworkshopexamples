#include <iostream>
#include <vector>

// Two SBO-like element types that differ ONLY in whether their move constructor
// is noexcept. (Stand-ins for a real SBO wrapper, which owns an inline buffer.)
struct GoodMove {
    int v;
    GoodMove(int x = 0) : v(x) {}
    GoodMove(GoodMove&& o) noexcept : v(o.v) { ++moves; }
    GoodMove(const GoodMove& o) : v(o.v) { ++copies; }
    static int moves, copies;
};
int GoodMove::moves = 0;
int GoodMove::copies = 0;

struct BadMove {
    int v;
    BadMove(int x = 0) : v(x) {}
    BadMove(BadMove&& o) : v(o.v) { ++moves; }        // move NOT marked noexcept
    BadMove(const BadMove& o) : v(o.v) { ++copies; }
    static int moves, copies;
};
int BadMove::moves = 0;
int BadMove::copies = 0;

int main() {
    // Force one reallocation each: reserve 2, then push a 3rd element.
    std::vector<GoodMove> g;
    g.reserve(2);
    g.emplace_back(1);
    g.emplace_back(2);
    GoodMove::moves = GoodMove::copies = 0;
    g.emplace_back(3);   // reallocation relocates the existing 2 elements
    std::cout << "noexcept move -> vector MOVES on realloc: moves="
              << GoodMove::moves << " copies=" << GoodMove::copies << '\n';   // moves=2 copies=0

    std::vector<BadMove> b;
    b.reserve(2);
    b.emplace_back(1);
    b.emplace_back(2);
    BadMove::moves = BadMove::copies = 0;
    b.emplace_back(3);   // reallocation
    std::cout << "throwing move  -> vector COPIES on realloc: moves="
              << BadMove::moves << " copies=" << BadMove::copies << '\n';     // moves=0 copies=2

    // An SBO type whose move can throw (or is simply not marked noexcept) makes
    // every vector reallocation copy instead of move -- exactly move_if_noexcept.
    // Mark the SBO move constructor noexcept whenever its element moves are.
    return 0;
}
