// Each member sits at an offset that is a multiple of its own alignment, so the
// compiler pads between them. Ordering fields from strictest to loosest
// alignment squeezes the padding out -- the same fields, a smaller struct.

#include <cstddef>   // offsetof
#include <iostream>

struct Bad {
    char   a;   // offset 0, then 7 bytes of padding
    double b;   // offset 8
    char   c;   // offset 16, then 7 bytes of tail padding
};

struct Good {
    double b;   // offset 0
    char   a;   // offset 8
    char   c;   // offset 9, then 6 bytes of tail padding
};

int main() {
    std::cout << "sizeof(Bad)  = " << sizeof(Bad)  << '\n';
    std::cout << "sizeof(Good) = " << sizeof(Good) << '\n';

    std::cout << "Bad::b  at offset " << offsetof(Bad, b) << '\n';
    std::cout << "Good::a at offset " << offsetof(Good, a) << '\n';
    std::cout << "same fields, " << (sizeof(Bad) - sizeof(Good))
              << " bytes saved by reordering\n";
}
