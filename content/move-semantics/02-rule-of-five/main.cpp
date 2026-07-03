#include <iostream>
#include <utility>

#include "intarray.hpp"

int main() {
    IntArray a(3);
    a.set(0, 10);

    IntArray b(a);              // copy constructor
    IntArray c(std::move(a));   // move constructor (a is now empty but valid)

    b = c;                       // copy assignment
    c = std::move(b);            // move assignment

    std::cout << "c[0]=" << c.get(0) << " c.size=" << c.size() << '\n';
    return 0;
}
