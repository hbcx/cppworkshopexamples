#include <functional>
#include <iostream>

// A type that counts how many times it was copied, so bind's hidden copy is
// visible without any undefined behaviour. A move constructor is provided (and
// does not count) so only real copies show up.
struct Tracked {
    int id;
    static int copies;
    explicit Tracked(int i) : id(i) {}
    Tracked(const Tracked& other) : id(other.id) { ++copies; }
    Tracked(Tracked&& other) noexcept : id(other.id) {}
};
int Tracked::copies = 0;

void use(const Tracked& t) { std::cout << "use id=" << t.id << "\n"; }

int main() {
    using namespace std::placeholders;

    Tracked t{7};
    // ANTI-PATTERN: assuming bind keeps a REFERENCE to your object. It stores a
    // COPY of every bound argument by default. For a big object that is a silent,
    // repeated expense; and if you expected the bound call to see later changes,
    // it will not -- it holds its own snapshot.
    auto f = std::bind(use, t);   // t is COPIED into the bind here
    std::cout << "copies after bind: " << Tracked::copies << "\n";

    t.id = 99;                    // change the original AFTER binding
    f();                          // still prints 7 -- the bind holds its own copy
    std::cout << "original id is now " << t.id << "\n";

    // FIX: bind a reference explicitly with std::ref -- no copy, and the call
    // sees the current value. Valid only while the referent outlives the bind.
    Tracked::copies = 0;
    Tracked u{1};
    auto g = std::bind(use, std::ref(u));
    std::cout << "copies with std::ref: " << Tracked::copies << "\n";
    u.id = 42;
    g();                          // prints 42 -- the live object
    return 0;
}
