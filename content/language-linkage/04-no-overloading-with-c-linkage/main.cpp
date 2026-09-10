#include <iostream>

// C++ functions are name-mangled, so these two OVERLOADS coexist: the mangled
// symbol encodes the parameter types, giving each its own name at link time.
int describe(int x)    { return x * 10; }
int describe(double x) { return static_cast<int>(x) + 1; }

// An extern "C" function is NOT mangled -- its symbol is just the bare name -- so
// two of them cannot share a name. This would be an error:
//
//   extern "C" int value(int);
//   extern "C" int value(double);   // error: conflicting C-linkage declarations
//
// A single extern "C" function is fine; overloading needs C++ linkage.
extern "C" int value(int x) { return x + 100; }

int main() {
    std::cout << "describe(3)   = " << describe(3) << "\n";     // 30  (int overload)
    std::cout << "describe(3.0) = " << describe(3.0) << "\n";   // 4   (double overload)
    std::cout << "value(7)      = " << value(7) << "\n";        // 107 (C linkage, no overload)
    return 0;
}
