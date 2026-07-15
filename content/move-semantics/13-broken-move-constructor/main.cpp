#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

// A stand-in "heap" so we can SHOW a double free deterministically without
// actually invoking undefined behaviour. Each id is one owned resource.
static std::vector<bool> gLive;

static int acquire() {
    gLive.push_back(true);
    int id = static_cast<int>(gLive.size()) - 1;
    std::cout << "  acquire #" << id << "\n";
    return id;
}

static void release(int id) {
    if (id < 0) return;                                   // moved-from / empty: nothing to free
    if (!gLive[static_cast<std::size_t>(id)]) {
        std::cout << "  DOUBLE FREE of #" << id << "  <-- bug\n";
        return;
    }
    gLive[static_cast<std::size_t>(id)] = false;
    std::cout << "  release #" << id << "\n";
}

// ANTI-PATTERN: the move constructor steals the id but forgets to invalidate the
// source. Both objects now believe they own the same resource, so both free it
// at scope exit -- a double free (a double delete / dangling pointer in real
// code). It compiles cleanly and often "works" until the destructors run.
struct BrokenBuffer {
    int id;
    BrokenBuffer() : id(acquire()) {}
    BrokenBuffer(BrokenBuffer&& o) noexcept : id(o.id) { /* forgot: o.id = -1; */ }
    ~BrokenBuffer() { release(id); }
};

// Correct: after stealing the resource, reset the source to an empty state.
struct FixedBuffer {
    int id;
    FixedBuffer() : id(acquire()) {}
    FixedBuffer(FixedBuffer&& o) noexcept : id(o.id) { o.id = -1; }
    ~FixedBuffer() { release(id); }
};

int main() {
    std::cout << "broken move ctor ->\n";
    {
        BrokenBuffer a;
        BrokenBuffer b = std::move(a);   // b steals a's id; a still holds it too
    }                                    // ~b then ~a -> the same id is freed twice

    std::cout << "fixed move ctor ->\n";
    {
        FixedBuffer a;
        FixedBuffer b = std::move(a);    // a is reset to -1
    }                                    // ~b frees once; ~a is a no-op

    return 0;
}
