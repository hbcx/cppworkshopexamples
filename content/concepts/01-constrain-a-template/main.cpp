#include <concepts>
#include <iostream>

// Constrain the template so only integer types are accepted. A call with the
// wrong type is rejected at the CALL SITE with a short message, instead of a
// wall of errors from deep inside the function body.
template <std::integral T>
T twice(T x) { return x + x; }

int main() {
    std::cout << twice(21) << "\n";      // ok: int satisfies std::integral
    std::cout << twice(-4) << "\n";

    // twice(3.14);   // ERROR: double does not satisfy std::integral -- the
    //                // compiler says so right here, not inside twice().
    std::cout << "done\n";
}
