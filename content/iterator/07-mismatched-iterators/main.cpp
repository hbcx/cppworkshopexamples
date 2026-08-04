// Anti-pattern: mixing iterators from different ranges.
//
// An iterator pair only means anything when both come from the SAME container
// and in the right order (begin then end). Pairing begin() of one container with
// end() of another, or swapping begin and end, produces a range that is not a
// range: the loop or algorithm walks off into unrelated memory (or never
// terminates), which is undefined behaviour. The mismatches are described in
// comments; the correct same-container calls run.

#include <iostream>
#include <vector>
#include <algorithm> // std::find

int main() {
    std::vector<int> a{ 1, 2, 3 };
    std::vector<int> b{ 4, 5, 6 };

    // ---- BAD (undefined behaviour -- described, NOT run) ----
    //
    //   std::find(a.begin(), b.end(), 2);   // a.begin() and b.end() are unrelated;
    //                                        // "a.begin() != b.end()" is never a valid test
    //
    //   for (auto it = a.begin(); it != b.end(); ++it) { ... } // walks off the end of a
    //
    //   std::find(a.end(), a.begin(), 2);   // reversed: end() before begin(), UB
    //
    // Iterators into different containers are not comparable, and the loop
    // condition it != b.end() is never satisfied by walking a, so it runs past
    // a's storage. None of this is diagnosable at compile time -- the types match.

    // ---- GOOD: both iterators from the same container, begin then end ----
    auto it = std::find(a.begin(), a.end(), 2);
    std::cout << "find 2 in a: " << (it != a.end() ? "yes" : "no") << "\n";

    auto jt = std::find(b.begin(), b.end(), 2);
    std::cout << "find 2 in b: " << (jt != b.end() ? "yes" : "no") << "\n";

    return 0;
}
