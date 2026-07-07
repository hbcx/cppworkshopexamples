#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Prints copy vs move vs move-assign so we can see which operation runs.
struct Tracer {
    std::string label;
    explicit Tracer(std::string l) : label(std::move(l)) {}
    Tracer(const Tracer& o) : label(o.label) { std::cout << "copy(" << label << ")\n"; }
    Tracer(Tracer&& o) noexcept : label(std::move(o.label)) { std::cout << "move(" << label << ")\n"; }
    Tracer& operator=(const Tracer& o) { std::cout << "copy-assign\n"; label = o.label; return *this; }
    Tracer& operator=(Tracer&& o) noexcept {
        std::cout << "move-assign\n";
        if (this != &o) { label = std::move(o.label); }  // guard makes self-move safe
        return *this;
    }
};

int main() {
    // (1) A moved-from object is VALID, just in an unspecified state: you may
    //     call any operation with no precondition -- assign to it, clear it,
    //     ask its size -- you just must not rely on a particular leftover value.
    std::string a = "hello";
    std::string b = std::move(a);
    std::cout << "b = " << b << "\n";
    a = "reusable after move";              // assigning to a moved-from object is fine
    std::cout << "a (reassigned) = " << a << "\n";

    // (2) std::move on a CONST object is a silent trap. std::move(c) has type
    //     const Tracer&&, which cannot bind to the move ctor (Tracer&&), so
    //     overload resolution falls back to the copy ctor. You pay for a copy.
    const Tracer c("payload");
    Tracer d = std::move(c);                // prints "copy", NOT "move"
    std::cout << "d.label via std::move(const) = " << d.label << "\n";

    // (3) Self-move must not corrupt the object. It happens in practice through
    //     aliasing (two names for one object). The (this != &o) guard turns the
    //     assignment into a no-op that leaves the value intact.
    Tracer e("keep-me");
    Tracer* alias = &e;
    e = std::move(*alias);                  // aliased self-move -> guarded no-op
    std::cout << "e.label after self-move = " << e.label << "\n";

    // (4) A moved-from container is guaranteed usable; a vector's move ctor
    //     leaves the source empty because it hands over its buffer.
    std::vector<int> src = {1, 2, 3};
    std::vector<int> dst = std::move(src);
    std::cout << "moved-to vector size = " << dst.size() << "\n";
    src.push_back(42);                       // still a valid object to grow
    std::cout << "source reused, size = " << src.size() << "\n";
    return 0;
}
