// Anti-pattern: an iterator that outlives its container.
//
// An iterator is a non-owning reference INTO a container: it stays valid only as
// long as that container lives. Take an iterator from a temporary, or from a
// local that goes out of scope, and the container is destroyed while the
// iterator still refers into it -- a dangling iterator, and using it is
// use-after-free. The dangling cases are described in comments; the fix is to
// keep the container alive in a named variable for as long as its iterators are
// used, which the runnable code does.

#include <iostream>
#include <vector>

static std::vector<int> makeNumbers() { return { 10, 20, 30 }; }

int main() {
    // ---- BAD (undefined behaviour -- described, NOT run) ----
    //
    //   auto it = makeNumbers().begin();
    //   std::cout << *it;   // the temporary vector was destroyed at the ';' above,
    //                        // so it dangles: use-after-free
    //
    //   const int* p;
    //   {
    //       std::vector<int> local{ 1, 2, 3 };
    //       p = &local[0];   // (an iterator/pointer into local)
    //   }                    // local destroyed here
    //   std::cout << *p;     // dangles: use-after-free
    //
    // A returned-by-value container is a temporary; iterators into it die with
    // it at the end of the full expression.

    // ---- GOOD: name the container so it outlives its iterators ----
    std::vector<int> nums = makeNumbers(); // materialised into a named variable
    auto it = nums.begin();                // nums outlives it
    std::cout << "first: " << *it << "\n";
    std::cout << "all:  ";
    for (int x : nums) std::cout << " " << x; // nums is alive for the whole loop
    std::cout << "\n";
    return 0;
}
