#include <algorithm>
#include <iostream>
#include <string>

// Returns a fresh string each call -- the result is a TEMPORARY at the call site.
static std::string label(int n) { return "item-" + std::to_string(n); }

int main() {
    // ANTI-PATTERN: keep a reference to the result of min/max when the arguments
    // were temporaries.
    //
    //   const std::string& picked = std::min(label(3), label(1));
    //
    // The two-argument std::min/std::max return a const REFERENCE to whichever
    // argument won -- they do not copy. Here both arguments are temporaries that
    // die at the end of the full expression, so `picked` would refer to freed
    // memory: reading it afterwards is undefined behaviour. It often "works" in a
    // quick test because the memory is not yet reused, which is what makes it
    // dangerous. We do not run that line.

    // FIX: bind by VALUE, so the winner is copied out before the temporaries die.
    std::string picked = std::min(label(3), label(1));
    std::cout << "picked (by value): " << picked << '\n';

    // Alternatively, keep the arguments alive in named variables and the
    // reference stays valid as long as they do.
    std::string a = label(5), b = label(2);
    const std::string& ref = std::max(a, b);   // a and b outlive ref -> safe
    std::cout << "max by reference:  " << ref << '\n';

    // Note: the initializer_list forms (std::min({...})) return by value, so they
    // do not have this trap -- only the two-argument forms return a reference.
    return 0;
}
