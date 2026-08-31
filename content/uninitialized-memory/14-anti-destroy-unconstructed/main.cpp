// Anti-pattern: destroying storage that holds no live object, or destroying the
// same range twice.
//
// destroy_at(p) is p->~T() -- it assumes p points at a live object. On raw
// bytes the destructor reads members that were never set; run twice, the second
// call works on already-dead storage. Both are undefined. The count you destroy
// must match the count you built, exactly once.

#include <iostream>
#include <memory>   // std::destroy_n, std::destroy_at

struct Tracked {
    int id;
    static int alive;
    explicit Tracked(int i) : id(i) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

int main() {
    const std::size_t n = 3;
    Tracked* buf = static_cast<Tracked*>(::operator new(sizeof(Tracked) * n));

    // BAD (undefined -- described, not run):
    //   std::destroy_n(buf, n);   // buf holds NO objects yet; ~Tracked() runs
    //                             // on raw bytes.
    // Also undefined: destroying twice --
    //   std::destroy_n(buf, n); std::destroy_n(buf, n);  // second call hits
    //                                                    // dead objects.

    // GOOD: construct the range, destroy it once, then free the bytes.
    for (std::size_t i = 0; i < n; ++i) new (buf + i) Tracked(static_cast<int>(i));
    std::cout << "after construct: alive = " << Tracked::alive << '\n';

    std::destroy_n(buf, n);   // exactly the objects we built, one time
    std::cout << "after destroy:   alive = " << Tracked::alive << '\n';

    ::operator delete(buf);
}
