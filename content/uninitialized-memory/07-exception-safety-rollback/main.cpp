// The strong exception guarantee. If an element's constructor throws while
// uninitialized_copy is filling raw storage, the algorithm destroys the
// objects it already built and then rethrows -- the destination is left with
// nothing half-constructed. The live-object counter proves the rollback.

#include <iostream>
#include <memory>       // std::uninitialized_copy
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
    std::cout << "source objects: alive = " << F::alive << '\n';

    F::throwAt = 3;   // the copy constructor will throw on id 3

    F* buf = static_cast<F*>(::operator new(sizeof(F) * src.size()));
    try {
        // Copies 0, 1, 2 succeed; copying id 3 throws. uninitialized_copy then
        // destroys those three partial copies before the exception escapes.
        std::uninitialized_copy(src.begin(), src.end(), buf);
        std::cout << "(not reached)\n";
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << '\n';
    }

    // Only the 5 source objects are left -- the partial copies rolled back.
    std::cout << "after the throw: alive = " << F::alive
              << " (partial copies rolled back)\n";

    // Nothing survived in buf, so just release the raw bytes.
    ::operator delete(buf);
}
