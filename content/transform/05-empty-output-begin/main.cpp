#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> in{1, 2, 3, 4};

    // BAD (shown in a comment -- writing through the begin() of an EMPTY vector is
    // undefined behavior):
    //
    //   std::vector<int> out;                       // empty: begin() == end()
    //   std::transform(in.begin(), in.end(), out.begin(),   // writes past the end -- UB
    //                  [](int x){ return x * x; });
    //
    // transform writes to consecutive positions starting at the output iterator; it
    // does NOT grow the container. out.begin() on an empty vector points at no
    // storage, so each write lands past the end -- memory corruption, not a resize.

    // FIX 1: a back_inserter appends, growing the output as it goes.
    std::vector<int> viaInserter;
    std::transform(in.begin(), in.end(), std::back_inserter(viaInserter),
        [](int x){ return x * x; });
    std::cout << "back_inserter: ";
    for (int x : viaInserter) std::cout << x << ' ';
    std::cout << '\n';

    // FIX 2: size the output first, then overwrite its elements in place.
    std::vector<int> sized(in.size());
    std::transform(in.begin(), in.end(), sized.begin(), [](int x){ return x * x; });
    std::cout << "pre-sized:     ";
    for (int x : sized) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
