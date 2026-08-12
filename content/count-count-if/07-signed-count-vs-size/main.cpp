#include <algorithm>
#include <vector>
#include <cstddef>
#include <iostream>

int main() {
    std::vector<int> nums{4, 4, 4, 4};

    // count returns a SIGNED integer (the iterator difference type, ptrdiff_t),
    // while size() returns an UNSIGNED type (size_t). Comparing them directly mixes
    // signedness.

    // BAD (shown in a comment -- it does not compile under -Wall -Werror, which is
    // exactly the warning doing its job):
    //
    //   auto c = std::count(nums.begin(), nums.end(), 4);   // signed (ptrdiff_t)
    //   if (c == nums.size()) { ... }                       // signed == unsigned -> -Wsign-compare
    //
    // The comparison converts the signed side to unsigned; for these values it
    // happens to work, but a negative signed operand would wrap to a huge unsigned
    // number and compare wrong -- the classic size-comparison bug.

    // GOOD: make both sides the same signedness -- cast size() to the signed type.
    auto c = std::count(nums.begin(), nums.end(), 4);
    bool allFours = c == static_cast<std::ptrdiff_t>(nums.size());
    std::cout << std::boolalpha;
    std::cout << "all fours (matched signedness)? " << allFours << '\n';   // true

    // BETTER: if the question is really are they all 4, ask all_of and skip the
    // count-versus-size comparison entirely.
    bool allFoursClearer = std::all_of(nums.begin(), nums.end(), [](int x){ return x == 4; });
    std::cout << "all fours (all_of)? " << allFoursClearer << '\n';         // true
    return 0;
}
