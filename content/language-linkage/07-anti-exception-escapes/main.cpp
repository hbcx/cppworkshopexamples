#include <iostream>

// ANTI-PATTERN (described): an extern "C" function that lets an exception escape.
// If C code calls this with b == 0, the C++ exception unwinds through the C
// caller's frame -- undefined behaviour, usually std::terminate:
//
//   extern "C" int divide(int a, int b) {
//       if (b == 0) throw std::runtime_error("no");   // escapes into C -> UB
//       return a / b;
//   }
//
// FIX: contain every failure inside the function and report it through a return
// code and an out-parameter. Marking it noexcept makes the boundary explicit --
// if anything did throw, the program terminates at the function instead of
// corrupting the C caller.
extern "C" int divide(int a, int b, int* out) noexcept {
    if (b == 0)
        return 1;              // error, no throw
    *out = a / b;
    return 0;
}

int main() {
    int r = 0;
    std::cout << "divide(9, 3): code " << divide(9, 3, &r) << ", result " << r << "\n";
    std::cout << "divide(9, 0): code " << divide(9, 0, &r) << " (error, nothing thrown)\n";
    return 0;
}
