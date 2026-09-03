#include <functional>
#include <iostream>

int main() {
    int a = 1;
    int b = 100;

    // A real reference is bound once; operator= on it ASSIGNS THROUGH to the
    // referent.
    int& ref = a;
    ref = 7;                      // a becomes 7
    std::cout << "plain reference: a = " << a << "\n";   // 7

    // A reference_wrapper's operator= is different: it REBINDS the wrapper to a
    // new referent -- it does NOT write through to the old one.
    std::reference_wrapper<int> w = std::ref(a);
    w = std::ref(b);              // w now refers to b; a is untouched
    std::cout << "after rebind: a = " << a << ", w -> " << w.get() << "\n";  // 7, 100

    // To write THROUGH a reference_wrapper you go via .get() (or the implicit
    // conversion to int&). This is the step people forget.
    w.get() = 55;                 // writes to b
    std::cout << "through get(): b = " << b << "\n";     // 55

    // So the rule is: w = something REBINDS the wrapper; w.get() = something
    // assigns to the referent.
    return 0;
}
