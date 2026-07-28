#include <iostream>
#include <memory>

// Anti-pattern: calling release() and expecting the object to be freed.
// release() does the OPPOSITE of reset(): it returns the raw pointer and gives
// up ownership WITHOUT deleting. A bare `p.release();` that ignores the result
// leaks. reset() is the call that deletes. We prove it by watching the dtor.
struct Tracked {
    int id;
    explicit Tracked(int i) : id(i) { std::cout << "Tracked " << id << " ctor\n"; }
    ~Tracked() { std::cout << "Tracked " << id << " dtor\n"; }
};

int main() {
    // release(): ownership leaves the unique_ptr, but NOTHING is deleted yet.
    std::unique_ptr<Tracked> p(new Tracked(1));
    Tracked* raw = p.release();          // p is now empty; object 1 is still alive
    std::cout << "after release: p owns " << (p ? "yes" : "no")
              << " (no dtor has run yet)\n";
    // A bare `p.release();` -- ignoring `raw` -- would leak object 1 here.
    // Because WE now hold the only pointer, WE must delete it (or pass it to a
    // new owner):
    delete raw;                          // object 1's dtor runs on this line

    // reset(): THIS is the call that deletes the managed object.
    std::unique_ptr<Tracked> q(new Tracked(2));
    std::cout << "calling reset...\n";
    q.reset();                           // object 2's dtor runs on this line
    std::cout << "after reset: q owns " << (q ? "yes" : "no") << '\n';
    return 0;
}
