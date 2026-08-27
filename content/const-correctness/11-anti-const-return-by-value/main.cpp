#include <iostream>
#include <string>
#include <vector>
#include <utility>

// Anti-pattern: returning a const value BY VALUE (const T, not const T&). It was
// once suggested to stop callers writing to a temporary, but since C++11 it
// backfires: a const prvalue cannot bind to a T&&, so it is COPIED where a move
// would have served. You lose move semantics for no real gain.

struct Tracer {
    std::string name;
    explicit Tracer(std::string n) : name(std::move(n)) {}
    Tracer(const Tracer& o) : name(o.name) {
        std::cout << "  COPY " << name << "\n";
    }
    Tracer(Tracer&& o) noexcept : name(std::move(o.name)) {
        std::cout << "  MOVE " << name << "\n";
    }
};

// BAD: const by value. The returned prvalue is const, so it cannot be moved.
const Tracer makeConst() { return Tracer("const-return"); }

// GOOD: plain by value. The returned prvalue can be moved.
Tracer makePlain() { return Tracer("plain-return"); }

int main() {
    std::vector<Tracer> v;
    v.reserve(2);               // no reallocation, so only our ctors print

    std::cout << "push_back(makeConst()):\n";
    v.push_back(makeConst());   // COPY: a const prvalue cannot bind to T&&
    std::cout << "push_back(makePlain()):\n";
    v.push_back(makePlain());   // MOVE

    std::cout << "stored: " << v[0].name << ", " << v[1].name << "\n";
    return 0;
}
