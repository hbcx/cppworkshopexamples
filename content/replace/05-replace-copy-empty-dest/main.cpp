#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> source{20, -1, 22, -1, 25};

    // ANTI-PATTERN: replace_copy into an empty destination through begin().
    //
    //   std::vector<int> out;                 // size 0, no room
    //   std::replace_copy(source.begin(), source.end(), out.begin(), -1, 0);
    //
    // replace_copy writes through the output iterator; it does NOT allocate. With
    // an empty vector, out.begin() == out.end(), so every write lands past the
    // end -- undefined behaviour. It often "works" briefly because the capacity
    // happens to be there, which hides the bug. We do not run that line.

    // FIX 1: a back_inserter grows the destination as elements are written.
    std::vector<int> out;
    std::replace_copy(source.begin(), source.end(),
                      std::back_inserter(out), -1, 0);
    print("via back_inserter:", out);

    // FIX 2: pre-size the destination to the input length, then write to begin().
    std::vector<int> sized(source.size());
    std::replace_copy(source.begin(), source.end(), sized.begin(), -1, 0);
    print("pre-sized dest:   ", sized);
    return 0;
}
