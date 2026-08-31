// A const member lets the compiler assume the value never changes. After reusing
// the storage for a new object, reading that member through the OLD pointer can
// return the stale value -- undefined. std::launder gives a pointer to the new
// object and blocks the assumption.

#include <iostream>
#include <new>

struct Tagged {
    const int id;        // const member: the reason launder is needed
    explicit Tagged(int i) : id(i) {}
};

int main() {
    alignas(Tagged) unsigned char storage[sizeof(Tagged)];

    Tagged* p = new (storage) Tagged(1);
    std::cout << "first: id = " << p->id << '\n';   // 1

    // Reuse the storage for a new Tagged. Imagine we did not capture the return
    // value and still hold only p.
    new (storage) Tagged(2);

    // BAD (undefined -- described, not run):
    //   std::cout << p->id;   // may print the stale 1: compiler assumes the
    //                         // const id it saw earlier is unchanged.

    // GOOD: launder p to get a pointer to the object that now lives there.
    Tagged* q = std::launder(p);
    std::cout << "after launder: id = " << q->id << '\n';   // 2

    q->~Tagged();
}
