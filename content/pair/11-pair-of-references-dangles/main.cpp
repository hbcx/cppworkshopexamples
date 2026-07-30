#include <iostream>
#include <string>
#include <utility>

std::string makeLabel() {
    return "temporary";
}

int main() {
    // Anti-pattern: a pair whose member is a reference, bound to a temporary.
    // A reference member does NOT extend the temporary's lifetime, so it dies at
    // the end of the full expression and .first is left dangling; reading it is
    // undefined behaviour. Not run -- UB is nondeterministic:
    //
    //     std::pair<const std::string&, int> bad(makeLabel(), 1);
    //     std::cout << bad.first;   // UB: makeLabel()'s result already destroyed
    //
    // make_pair(std::ref(x), 1) hides the very same reference member behind auto.

    // Fix: store by value. The pair owns a copy and stays valid as long as it
    // does -- no lifetime coupling to any temporary.
    std::pair<std::string, int> good(makeLabel(), 1);
    std::cout << "value : " << good.first << ", n = " << good.second << '\n';

    // If a reference member is truly wanted, keep the referent alive yourself:
    // a named object that outlives the pair, never a temporary.
    std::string kept = makeLabel();
    std::pair<const std::string&, int> ref(kept, 2);
    std::cout << "ref   : " << ref.first << ", n = " << ref.second << '\n';
    return 0;
}
