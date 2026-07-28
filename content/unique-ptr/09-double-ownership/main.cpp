#include <iostream>
#include <memory>
#include <utility>

// Anti-pattern: building two unique_ptrs from the SAME raw pointer. Each thinks
// it is the sole owner, so both delete the object -- a double free (undefined
// behaviour). We describe the bug, then run only the correct version, because
// executing the double free would be UB and could crash the build.
struct Tracked {
    int id;
    explicit Tracked(int i) : id(i) { std::cout << "Tracked " << id << " ctor\n"; }
    ~Tracked() { std::cout << "Tracked " << id << " dtor\n"; }
};

int main() {
    // --- the bug (NOT executed: it would double-delete, which is UB) ---
    // Tracked* raw = new Tracked(1);
    // std::unique_ptr<Tracked> a(raw);
    // std::unique_ptr<Tracked> b(raw);   // second owner of the SAME object
    // // at scope end: a deletes it, then b deletes it again -> double free.

    // --- the fix: exactly one owner, transferred explicitly ---
    std::unique_ptr<Tracked> a(new Tracked(1));
    std::unique_ptr<Tracked> b = std::move(a);   // ownership moves; a is now null
    std::cout << "a owns: " << (a ? "yes" : "no")
              << ", b owns: " << (b ? "yes" : "no") << '\n';

    // --- best: make_unique (C++14) never exposes a loose raw pointer to alias ---
    // auto c = std::make_unique<Tracked>(2);
    std::unique_ptr<Tracked> c(new Tracked(2));
    std::cout << "c owns Tracked " << c->id << '\n';
    return 0;   // exactly one dtor for Tracked 1 and one for Tracked 2
}
