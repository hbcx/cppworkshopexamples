#include <functional>
#include <iostream>

int main() {
    int target = 1;
    int source = 42;

    std::reference_wrapper<int> w = std::ref(target);

    // ANTI-PATTERN: writing `w = source` expecting to set the referent (target)
    // to source's value -- as `int& r = target; r = source;` would. It does not.
    // source is an int lvalue, so it converts to a reference_wrapper<int> and
    // operator= REBINDS w to refer to source; target is left unchanged.
    w = source;
    std::cout << "after w = source: target = " << target
              << ", w -> " << w.get() << "\n";   // target = 1, w -> 42

    // FIX: assign THROUGH the referent with .get() (or the implicit conversion).
    std::reference_wrapper<int> w2 = std::ref(target);
    w2.get() = source;    // writes source's value into target
    std::cout << "after w2.get() = source: target = " << target << "\n";   // 42
    return 0;
}
