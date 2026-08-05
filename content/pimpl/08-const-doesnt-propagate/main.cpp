#include "counter.hpp"

#include <iostream>

int main() {
    const Counter c; // a CONST object

    c.peekBad(); // yet these const calls mutate it
    c.peekBad();
    std::cout << "after 2x peekBad on a const Counter: reads=" << c.reads()
              << " (const silently violated)\n";

    const Counter d;
    d.lookGood();
    d.lookGood();
    std::cout << "after 2x lookGood on a const Counter: reads=" << d.reads()
              << " (const respected)\n";
}
