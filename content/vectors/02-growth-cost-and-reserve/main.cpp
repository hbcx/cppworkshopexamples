#include <cstddef>
#include <iostream>
#include <vector>

// An element that reports how often it is move- or copy-constructed. Growth
// relocates every existing element into the new block, so the move count is the
// hidden cost of letting a vector grow one element at a time.
struct Elem {
    int value;
    static int moves;   // move-constructions (how growth relocates elements)
    static int copies;  // copy-constructions (stays 0: our move is noexcept)

    explicit Elem(int v) : value(v) {}
    Elem(const Elem& o) : value(o.value) { ++copies; }
    Elem(Elem&& o) noexcept : value(o.value) { ++moves; }
};
int Elem::moves = 0;
int Elem::copies = 0;

static void fill(bool withReserve) {
    Elem::moves = 0;
    Elem::copies = 0;

    std::vector<Elem> v;
    if (withReserve) {
        v.reserve(8);
    }

    int reallocations = 0;
    std::size_t cap = v.capacity();
    for (int i = 0; i < 8; ++i) {
        v.push_back(Elem(i));
        if (v.capacity() != cap) {  // capacity changed => the block was reallocated
            ++reallocations;
            cap = v.capacity();
        }
    }

    std::cout << (withReserve ? "with reserve(8):  " : "without reserve:  ")
              << "reallocations=" << reallocations
              << " element-moves=" << Elem::moves
              << " element-copies=" << Elem::copies << '\n';
}

int main() {
    fill(false);
    fill(true);
    std::cout << "Reserving the final size up front removed every reallocation.\n";
    return 0;
}
