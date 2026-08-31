// Anti-pattern: filling raw storage with a hand-written placement-new loop.
//
// On the happy path it matches uninitialized_copy. But when a constructor
// throws midway, the loop unwinds with its already-built prefix still alive and
// now unreachable -- a leak. uninitialized_copy destroys that prefix before
// rethrowing. The live-object counter shows the difference.

#include <iostream>
#include <memory>       // std::uninitialized_copy, std::destroy_n
#include <stdexcept>
#include <vector>

struct Fragile {
    int id;
    static int alive;
    static int throwAt;
    explicit Fragile(int i) : id(i) { ++alive; }
    Fragile(const Fragile& o) : id(o.id) {
        if (id == throwAt) throw std::runtime_error("copy failed on id 3");
        ++alive;
    }
    ~Fragile() { --alive; }
};
int Fragile::alive = 0;
int Fragile::throwAt = -1;

int main() {
    using F = Fragile;

    std::vector<F> src;
    for (int i = 0; i < 5; ++i) src.emplace_back(i);   // 5 live source objects
    F::throwAt = 3;

    // BAD: manual placement-new loop. No cleanup when the copy throws.
    {
        F* buf = static_cast<F*>(::operator new(sizeof(F) * src.size()));
        int built = 0;
        try {
            for (std::size_t i = 0; i < src.size(); ++i) {
                new (buf + i) F(src[i]);   // throws while copying id 3
                ++built;
            }
        } catch (const std::exception& e) {
            std::cout << "manual loop threw: " << e.what() << '\n';
            // buf[0..built-1] are constructed and now orphaned -- the loop
            // does nothing to destroy them.
        }
        std::cout << "after manual loop: alive = " << F::alive
                  << " (source 5 + " << built << " orphaned)\n";
        std::destroy_n(buf, built);   // clean up so the demo stays even
        ::operator delete(buf);
    }

    // GOOD: uninitialized_copy destroys the prefix it built before rethrowing.
    {
        F* buf = static_cast<F*>(::operator new(sizeof(F) * src.size()));
        try {
            std::uninitialized_copy(src.begin(), src.end(), buf);
        } catch (const std::exception& e) {
            std::cout << "uninitialized_copy threw: " << e.what() << '\n';
        }
        std::cout << "after algorithm:   alive = " << F::alive
                  << " (source 5, copies rolled back)\n";
        ::operator delete(buf);   // nothing constructed survived
    }
}
