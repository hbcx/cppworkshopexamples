#include <iostream>
#include "api.h"

// The definitions match the extern "C" declarations in api.h, so they inherit C
// linkage from those declarations -- no need to repeat extern "C" here.
int c_add(int a, int b) { return a + b; }
int c_mul(int a, int b) { return a * b; }

int main() {
    std::cout << "c_add(4, 5) = " << c_add(4, 5) << "\n";
    std::cout << "c_mul(4, 5) = " << c_mul(4, 5) << "\n";
    return 0;
}
