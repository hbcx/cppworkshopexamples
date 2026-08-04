// Anti-pattern: dereferencing end() (using a find result unchecked).
//
// The algorithms that search -- std::find, std::find_if, and a container's own
// find() -- report "not found" by returning the end iterator. end() does not
// point at an element; it is one past the last, a position, not a value.
// Dereferencing it is undefined behaviour. The rule is to compare the returned
// iterator against end() BEFORE dereferencing it. The unchecked deref is
// described in a comment and not run; the checked version runs.

#include <iostream>
#include <vector>
#include <algorithm> // std::find

int main() {
    std::vector<int> v{ 1, 2, 3 };

    // Search for a value that is present, and one that is not.
    auto hit  = std::find(v.begin(), v.end(), 2); // found -> points at the 2
    auto miss = std::find(v.begin(), v.end(), 9); // not found -> returns end()

    // ---- BAD (undefined behaviour -- described, NOT run) ----
    //
    //   std::cout << *miss;   // miss == end(): dereferences one-past-the-last -> UB
    //
    // It often prints a believable-looking garbage number, which is what makes it
    // dangerous: the bug hides until the value happens to matter.

    // ---- GOOD: check against end() first ----
    if (hit != v.end())
        std::cout << "found: " << *hit << "\n";
    else
        std::cout << "not found\n";

    if (miss != v.end())
        std::cout << "found: " << *miss << "\n";
    else
        std::cout << "not found\n";

    return 0;
}
