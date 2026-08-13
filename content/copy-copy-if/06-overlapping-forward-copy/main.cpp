#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // Goal: shift the first four elements one position to the RIGHT within the same
    // vector, so 1 2 3 4 5 becomes 1 1 2 3 4 (the leading 1 stays, the last is lost).

    // BAD (shown in a comment -- copying FORWARD into a destination that overlaps
    // AHEAD of the source overwrites elements before they are read):
    //
    //   std::vector<int> v{1, 2, 3, 4, 5};
    //   std::copy(v.begin(), v.begin() + 4, v.begin() + 1);   // dest overlaps ahead
    //   // it reads v[0]=1 and writes v[1]; then reads v[1] -- already 1, not the
    //   // old 2 -- so the first value smears across, giving 1 1 1 1 1, not a shift.
    //
    // std::copy requires that the destination NOT overlap a source region it has not
    // read yet; a destination ahead of the source breaks that -- undefined behavior.

    // GOOD: copy_backward copies from the END toward the front, so each element is
    // read before the write that would overwrite it. This is the correct right shift.
    std::vector<int> v{1, 2, 3, 4, 5};
    std::copy_backward(v.begin(), v.begin() + 4, v.end());   // move the first 4 right by one
    std::cout << "shifted right with copy_backward: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';   // 1 1 2 3 4
    return 0;
}
