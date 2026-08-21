#include <iostream>
#include <span>
#include <vector>

int main() {
    // Anti-pattern: writing `const std::span<int>` expecting the ELEMENTS to be
    // read-only. const applies to the span object (you cannot re-point it); the
    // elements it views stay writable. Read-only elements need span<const int>.
    std::vector<int> v{1, 2, 3};

    const std::span<int> cs = v; // a CONST span of (mutable) int
    cs[0] = 99;                  // compiles and MUTATES v -- the surprise
    std::cout << "through const span<int>, v[0] = " << v[0] << '\n';

    std::span<const int> sc = v; // a span of CONST int -> elements read-only
    // sc[0] = 7;                // this would NOT compile
    std::cout << "span<const int> can read v[0] = " << sc[0] << '\n';

    // Rule: put const INSIDE the angle brackets to protect the data:
    //   span<const int>  -> like const int*  (read-only elements)
    //   const span<int>  -> like int* const  (fixed view, writable elements)
    return 0;
}
