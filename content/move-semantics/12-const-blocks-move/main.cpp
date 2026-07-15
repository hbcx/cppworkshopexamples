#include <iostream>
#include <utility>

struct Tracked {
    Tracked() = default;
    Tracked(const Tracked&) { std::cout << "  COPIED\n"; }
    Tracked(Tracked&&) noexcept { std::cout << "  MOVED\n"; }
    Tracked& operator=(const Tracked&) { std::cout << "  copy-assigned\n"; return *this; }
    Tracked& operator=(Tracked&&) noexcept { std::cout << "  move-assigned\n"; return *this; }
};

// ANTI-PATTERN: 'const' on a by-value return type. It looks harmless (even
// "safer"), but the returned prvalue is const, so it can never bind to a move
// operation at the call site -- every assignment of the result copies.
const Tracked makeConst() { return Tracked{}; }

// The same factory without the const return qualifier.
Tracked makePlain() { return Tracked{}; }

int main() {
    // Copy elision hides the bug: initialization elides everything, so the const
    // return looks free. The penalty only appears once you assign the result
    // into an existing object.
    std::cout << "initialize from const factory (elided, nothing printed) ->\n";
    [[maybe_unused]] Tracked fresh = makeConst();

    Tracked dst;

    std::cout << "assign from const factory ->\n";
    dst = makeConst();     // const prvalue -> move-assign cannot bind -> COPY-assigned

    std::cout << "assign from plain factory ->\n";
    dst = makePlain();     // move-assigned

    return 0;
}
