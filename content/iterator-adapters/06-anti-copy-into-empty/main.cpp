#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> src{1, 2, 3, 4};

    // ANTI-PATTERN: copying into an EMPTY container using its begin() as the
    // output. std::copy writes THROUGH the output iterator and never grows the
    // destination, so this writes past the end of an empty vector -- undefined
    // behaviour (a buffer overrun), even if it seems to "work" sometimes:
    //
    //   std::vector<int> dst;                              // size 0, no slots
    //   std::copy(src.begin(), src.end(), dst.begin());    // UB: writes dst[0..3]
    //
    // The output iterator needs somewhere real to write. Two correct fixes:

    // FIX 1: back_inserter appends, growing the container as it goes.
    std::vector<int> viaInserter;
    std::copy(src.begin(), src.end(), std::back_inserter(viaInserter));
    std::cout << "back_inserter size = " << viaInserter.size() << "\n";   // 4

    // FIX 2: pre-size the destination so real, writable slots exist first.
    std::vector<int> viaResize(src.size());
    std::copy(src.begin(), src.end(), viaResize.begin());
    std::cout << "pre-sized size = " << viaResize.size() << "\n";         // 4
    std::cout << "viaResize.back() = " << viaResize.back() << "\n";       // 4
    return 0;
}
