#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> src{1, 2, 3, 4, 5};

    // BAD: duplicating a WHOLE container with std::copy into an empty one through a
    // back_inserter. It works, but it is more code than the job needs and grows the
    // output element by element (repeated reallocations) instead of sizing it once.
    std::vector<int> viaCopy;
    std::copy(src.begin(), src.end(), std::back_inserter(viaCopy));

    // GOOD: to copy a whole container, use the copy constructor or assignment. It
    // allocates once and states the intent plainly.
    std::vector<int> viaCtor = src;   // copy constructor
    std::vector<int> viaAssign;
    viaAssign = src;                  // copy assignment

    // For a SUB-RANGE into an existing vector, assign from iterators -- it sizes
    // once, then copies, unlike a back_inserter growing step by step.
    std::vector<int> firstThree;
    firstThree.assign(src.begin(), src.begin() + 3);

    std::cout << "viaCopy: " << viaCopy.size()
              << ", viaCtor: " << viaCtor.size()
              << ", viaAssign: " << viaAssign.size()
              << ", firstThree: ";
    for (int x : firstThree) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
