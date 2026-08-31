// The destroy family runs destructors on objects living in raw storage without
// freeing the bytes:
//   destroy_at(p)        -> p->~T()
//   destroy_n(first, n)  -> destroy n objects, return one past the last
//   destroy(first, last) -> destroy a whole range
// A live-object counter shows each call ending exactly the objects you name.

#include <iostream>
#include <memory>   // std::destroy, std::destroy_n, std::destroy_at

struct Tracked {
    int id;
    static int alive;
    explicit Tracked(int i) : id(i) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

int main() {
    const std::size_t n = 4;
    Tracked* buf = static_cast<Tracked*>(::operator new(sizeof(Tracked) * n));

    // Build 4 objects in the raw storage with placement new.
    for (std::size_t i = 0; i < n; ++i) new (buf + i) Tracked(static_cast<int>(i));
    std::cout << "after construct: alive = " << Tracked::alive << '\n';

    // End them a few different ways.
    std::destroy_at(buf + 0);           // one object
    std::cout << "after destroy_at:  alive = " << Tracked::alive << '\n';

    std::destroy_n(buf + 1, 2);         // a counted run of two
    std::cout << "after destroy_n:   alive = " << Tracked::alive << '\n';

    std::destroy(buf + 3, buf + 4);     // the remaining range
    std::cout << "after destroy:     alive = " << Tracked::alive << '\n';

    // All objects are gone; only the raw bytes remain to be released.
    ::operator delete(buf);
}
