#include <iostream>

// ANTI-PATTERN (described, does not compile): two extern "C" functions with the
// same name. C linkage means no mangling, so both would be the symbol "scale" and
// nothing distinguishes them:
//
//   extern "C" int    scale(int x)    { return x * 2; }
//   extern "C" double scale(double x) { return x * 2.0; }   // error: conflicting
//                                                            // C-linkage declarations
//
// FIX (a): give the C-linkage functions distinct names -- the C convention.
extern "C" int    scale_i(int x)    { return x * 2; }
extern "C" double scale_d(double x) { return x * 2.0; }

// FIX (b): if you really want overloading, keep C++ linkage (the default) and let
// name mangling tell the two apart.
int    scale(int x)    { return x * 2; }
double scale(double x) { return x * 2.0; }

int main() {
    std::cout << "scale_i(21)  = " << scale_i(21) << "\n";
    std::cout << "scale_d(2.5) = " << scale_d(2.5) << "\n";
    std::cout << "scale(21)    = " << scale(21) << "\n";     // C++ overload
    std::cout << "scale(2.5)   = " << scale(2.5) << "\n";    // C++ overload
    return 0;
}
