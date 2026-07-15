#include <iostream>
#include <utility>
#include <vector>

// A member whose copy and move are observable, so we can see which one the
// compiler-generated special members of the OUTER class actually call.
struct Tracked {
    Tracked() = default;
    Tracked(const Tracked&) { std::cout << "  member COPIED\n"; }
    Tracked(Tracked&&) noexcept { std::cout << "  member MOVED\n"; }
    Tracked& operator=(const Tracked&) { std::cout << "  member copy-assigned\n"; return *this; }
    Tracked& operator=(Tracked&&) noexcept { std::cout << "  member move-assigned\n"; return *this; }
};

// ANTI-PATTERN: a user-declared destructor -- often added "just to log" or to
// set a breakpoint -- suppresses the implicitly generated MOVE constructor. The
// class stays copyable, so std::move silently falls back to a COPY.
struct WithDestructor {
    Tracked t;
    ~WithDestructor() {}          // this single line disables implicit moves
};

// Rule of zero: declare none of the special members and the compiler generates
// a real move constructor for you.
struct RuleOfZero {
    Tracked t;
};

int main() {
    std::cout << "WithDestructor: std::move ->\n";
    WithDestructor a;
    [[maybe_unused]] WithDestructor b = std::move(a);   // expected a move, but it COPIES

    std::cout << "RuleOfZero: std::move ->\n";
    RuleOfZero c;
    [[maybe_unused]] RuleOfZero d = std::move(c);        // really moves

    // The same trap hits container growth: a vector of WithDestructor copies
    // every element on reallocation instead of moving them.
    std::cout << "vector<WithDestructor> reallocation ->\n";
    std::vector<WithDestructor> v;
    v.reserve(1);
    v.emplace_back();             // fills the single reserved slot
    v.emplace_back();             // reallocates -> the first element is relocated by COPY

    return 0;
}
