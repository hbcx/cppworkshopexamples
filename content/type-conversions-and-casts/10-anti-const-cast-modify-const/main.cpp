#include <iostream>

int main() {
    // DEFINED: the real object (value) is NOT const. We only reached it through a
    // const pointer, so casting const away and writing is legal here.
    int value = 5;
    const int* view = &value;
    *const_cast<int*>(view) = 42;
    std::cout << "non-const object modified via const_cast: " << value << "\n";   // 42

    // UNDEFINED, described but NOT run:
    //   const int frozen = 5;
    //   *const_cast<int*>(&frozen) = 42;   // frozen is truly const -> undefined
    //   // may "work", may not; the compiler is allowed to assume frozen stays 5.

    // The honest fix when something must change is to not make it const:
    int changeable = 5;
    changeable = 42;               // no cast, no doubt
    std::cout << "the real fix -- a non-const variable: " << changeable << "\n";  // 42
    return 0;
}
