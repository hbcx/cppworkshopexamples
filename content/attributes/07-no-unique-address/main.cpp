#include <iostream>

// An empty type (no data members). On its own it still has size 1, but as a
// member it can be made to overlap with the others.
struct Empty {};

// Without the attribute, the empty member occupies its own byte, and alignment
// padding for the int pushes the struct out to 8 bytes.
struct WithoutAttr {
    Empty e;
    int value;
};

// With [[no_unique_address]], the empty member is allowed to take no space, so the
// struct is just its int.
struct WithAttr {
    [[no_unique_address]] Empty e;
    int value;
};

int main() {
    std::cout << "sizeof(int)         = " << sizeof(int) << "\n";
    std::cout << "sizeof(WithoutAttr) = " << sizeof(WithoutAttr) << "\n";
    std::cout << "sizeof(WithAttr)    = " << sizeof(WithAttr) << "\n";
    return 0;
}
