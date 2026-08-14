#include <algorithm>
#include <iostream>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    // Goal: shift the first four elements two places to the RIGHT within the same
    // vector, making room at the front. The source [0,4) and destination [2,6)
    // OVERLAP, so a forward std::copy would overwrite elements it has not read yet.
    std::vector<int> v{1, 2, 3, 4, 0, 0};

    // std::copy_backward copies [first, last) into a range ENDING at d_last,
    // working from the back toward the front. Because it writes the last element
    // first, the overlap is safe when shifting to the right.
    std::copy_backward(v.begin(), v.begin() + 4, v.end());
    print("after copy_backward:", v);   // 1 2 1 2 3 4 -- the block moved right by 2

    // The arguments name the END of the destination, not its start: the result
    // occupies [d_last - (last - first), d_last). Use copy_backward whenever the
    // destination is AHEAD of the source and they overlap; plain std::copy is
    // correct only when the destination is before the source or they do not touch.
    return 0;
}
