#include <iostream>

int liveCount = 0;   // open handles; releasing the same handle twice goes negative
int nextId = 1;

int acquire() { int id = nextId++; ++liveCount; std::cout << "  acquire #" << id << "\n"; return id; }
void release(int id) { --liveCount; std::cout << "  release #" << id << "\n"; }

// BAD: the destructor releases the handle, but the copy is the compiler's
// shallow copy -- it copies id_, so two owners release the SAME handle.
class BadOwner {
public:
    BadOwner() : id_(acquire()) {}
    ~BadOwner() { release(id_); }
    // no copy ctor / copy assignment declared -> shallow copy of id_ (the bug)
    int id_;
};

// GOOD: rule of three. The copy constructor acquires its OWN handle, matched
// by the destructor. (A full rule of three writes copy assignment too.)
class GoodOwner {
public:
    GoodOwner() : id_(acquire()) {}
    ~GoodOwner() { release(id_); }
    GoodOwner(const GoodOwner&) : id_(acquire()) {}
    GoodOwner& operator=(const GoodOwner&) = delete;
    int id_;
};

int main() {
    std::cout << "bad -- shallow copy shares the handle:\n";
    {
        BadOwner a;          // acquire #1
        BadOwner b = a;      // shallow copy: b.id_ == 1, no acquire
        (void)b;
    }                        // both destructors release #1 -> released twice
    std::cout << "liveCount after bad = " << liveCount << "  <- not 0, double free\n\n";

    liveCount = 0;           // isolate the good demo from the leak above

    std::cout << "good -- rule of three gives each its own handle:\n";
    {
        GoodOwner a;         // acquire #2
        GoodOwner b = a;     // copy ctor: acquire #3
        (void)b;
    }                        // release #3, release #2
    std::cout << "liveCount after good = " << liveCount << "  <- back to 0\n";
    return 0;
}
