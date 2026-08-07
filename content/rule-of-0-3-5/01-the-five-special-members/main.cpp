#include <iostream>
#include <utility>

// A tiny type that announces which special member runs. In a real class you
// would NOT write these -- they exist here only to make the five visible.
struct Tracked {
    Tracked()                            { std::cout << "default ctor\n"; }
    Tracked(const Tracked&)              { std::cout << "copy ctor\n"; }
    Tracked(Tracked&&) noexcept          { std::cout << "move ctor\n"; }
    Tracked& operator=(const Tracked&)   { std::cout << "copy assign\n"; return *this; }
    Tracked& operator=(Tracked&&) noexcept { std::cout << "move assign\n"; return *this; }
    ~Tracked()                           { std::cout << "destructor\n"; }
};

int main() {
    std::cout << "-- Tracked a --\n";
    Tracked a;                    // default ctor
    std::cout << "-- Tracked b = a --\n";
    Tracked b = a;                // copy ctor  (a is an lvalue)
    std::cout << "-- Tracked c = std::move(a) --\n";
    Tracked c = std::move(a);     // move ctor  (the argument is an rvalue)
    std::cout << "-- b = c --\n";
    b = c;                        // copy assignment
    std::cout << "-- b = std::move(c) --\n";
    b = std::move(c);             // move assignment
    std::cout << "-- end of scope --\n";
    return 0;                     // three destructors, in reverse order
}
