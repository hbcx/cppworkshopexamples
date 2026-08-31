// uninitialized_move: move-construct a range into raw storage. This is the
// heart of a container regrow -- allocate a bigger block, relocate the live
// objects into it by moving (not copying), then destroy the old range.

#include <iostream>
#include <memory>   // std::uninitialized_move, std::destroy
#include <string>
#include <utility>

struct Widget {
    std::string name;
    static int moves;
    explicit Widget(std::string n) : name(std::move(n)) {}
    Widget(Widget&& o) noexcept : name(std::move(o.name)) { ++moves; }
    Widget(const Widget&) = default;
    Widget& operator=(Widget&&) = default;
};
int Widget::moves = 0;

int main() {
    using W = Widget;

    // A small "old block" of 3 live widgets, built in raw storage.
    const std::size_t oldSize = 3;
    W* oldBlock = static_cast<W*>(::operator new(sizeof(W) * oldSize));
    new (oldBlock + 0) W("engine");
    new (oldBlock + 1) W("gearbox");
    new (oldBlock + 2) W("axle");

    // Grow: a bigger block, then MOVE every live object across.
    const std::size_t newCap = 6;
    W* newBlock = static_cast<W*>(::operator new(sizeof(W) * newCap));
    std::uninitialized_move(oldBlock, oldBlock + oldSize, newBlock);

    std::cout << "relocated by moving; move constructions: " << W::moves << '\n';
    for (W* p = newBlock; p != newBlock + oldSize; ++p) {
        std::cout << "  " << p->name << '\n';
    }

    // The source objects are moved-from but still alive: destroy them, then
    // free the old block.
    std::destroy(oldBlock, oldBlock + oldSize);
    ::operator delete(oldBlock);

    // Clean up the new block too.
    std::destroy(newBlock, newBlock + oldSize);
    ::operator delete(newBlock);
}
