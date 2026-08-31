// Anti-pattern: releasing raw storage without destroying the objects in it.
//
// operator delete frees the BYTES only -- it never runs destructors. Objects
// built with placement new must be destroyed by hand first, or everything they
// own leaks. The live-object counter makes the leak visible.

#include <iostream>
#include <memory>   // std::destroy

struct Tracked {
    int id;
    static int alive;
    explicit Tracked(int i) : id(i) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

int main() {
    const std::size_t n = 3;

    // BAD: construct, then free the bytes WITHOUT destroying.
    {
        Tracked* buf = static_cast<Tracked*>(::operator new(sizeof(Tracked) * n));
        for (std::size_t i = 0; i < n; ++i) new (buf + i) Tracked(static_cast<int>(i));
        ::operator delete(buf);   // destructors never ran
        std::cout << "bad  (skipped destroy): alive = " << Tracked::alive
                  << " (leaked)\n";
    }

    Tracked::alive = 0;   // reset the counter for the second demo

    // GOOD: destroy the range first, then free the bytes.
    {
        Tracked* buf = static_cast<Tracked*>(::operator new(sizeof(Tracked) * n));
        for (std::size_t i = 0; i < n; ++i) new (buf + i) Tracked(static_cast<int>(i));
        std::destroy(buf, buf + n);
        ::operator delete(buf);
        std::cout << "good (destroyed first): alive = " << Tracked::alive << '\n';
    }
}
