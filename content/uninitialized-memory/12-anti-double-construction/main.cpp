// Anti-pattern: running a construction algorithm over storage that already
// holds live objects.
//
// uninitialized_fill assumes raw slots. If the slots already hold objects, it
// constructs new ones on top: the old objects are gone but their destructors
// never ran, so anything they owned leaks. The counter shows the count double.

#include <iostream>
#include <memory>   // std::uninitialized_fill, std::destroy

struct Tracked {
    int id;
    static int alive;
    explicit Tracked(int i) : id(i) { ++alive; }
    Tracked(const Tracked& o) : id(o.id) { ++alive; }   // fill copies, so count copies too
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

int main() {
    const std::size_t n = 3;
    Tracked* buf = static_cast<Tracked*>(::operator new(sizeof(Tracked) * n));

    // Slots now hold 3 live objects.
    std::uninitialized_fill(buf, buf + n, Tracked(1));
    std::cout << "after first fill:  alive = " << Tracked::alive << '\n';

    // BAD: fill again over the SAME live slots. The 3 old objects are
    // overwritten with no destructor call -- they leak.
    std::uninitialized_fill(buf, buf + n, Tracked(2));
    std::cout << "after second fill: alive = " << Tracked::alive
              << " (3 old objects leaked)\n";

    // GOOD next time: destroy the live range before refilling it.
    //   std::destroy(buf, buf + n);
    //   std::uninitialized_fill(buf, buf + n, Tracked(2));
    // Here we just clean up the 3 objects that currently live in buf.
    std::destroy(buf, buf + n);
    ::operator delete(buf);
    std::cout << "after cleanup:     alive = " << Tracked::alive
              << " (the leaked 3 are unreachable)\n";
}
