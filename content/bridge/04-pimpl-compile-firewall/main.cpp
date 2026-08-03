#include "widget.hpp"

#include <utility>

int main() {
    Widget w("panel");
    w.greet();                       // greeting #1
    w.greet();                       // greeting #2

    // A pimpl type moves cheaply -- moving it is just swapping the one pointer,
    // and the hidden state travels with it.
    Widget moved = std::move(w);
    moved.greet();                   // greeting #3 -- state carried across the move
    return 0;
}
