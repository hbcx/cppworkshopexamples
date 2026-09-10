#include <iostream>

// extern "C" gives this function C language linkage: its symbol is NOT name-
// mangled, so C code (or another language, or a dynamic loader) can find and call
// it by the plain name "add_ints". The body is still ordinary C++.
extern "C" int add_ints(int a, int b) {
    return a + b;
}

int main() {
    // From C++ it is called like any other function; the only difference is the
    // linker-level name the compiler gives it.
    std::cout << "add_ints(2, 3) = " << add_ints(2, 3) << "\n";
    return 0;
}
