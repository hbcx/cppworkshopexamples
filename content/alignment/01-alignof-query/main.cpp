// alignof reports the boundary a type must be placed on. A struct inherits the
// strictest requirement among its members, because that member must stay aligned
// wherever the struct lands.

#include <iostream>

struct Mixed {
    char c;
    double d;   // the strictest member: forces Mixed to 8-byte alignment
    int i;
};

int main() {
    std::cout << "alignof(char)   = " << alignof(char)   << '\n';
    std::cout << "alignof(int)    = " << alignof(int)    << '\n';
    std::cout << "alignof(double) = " << alignof(double) << '\n';
    std::cout << "alignof(Mixed)  = " << alignof(Mixed)
              << "  (largest member wins)\n";
}
