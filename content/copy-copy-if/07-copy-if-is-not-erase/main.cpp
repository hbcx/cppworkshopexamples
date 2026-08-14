#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> nums{4, 7, 2, 9, 1, 8};

    // BAD idea: expecting copy_if to REMOVE the odds from nums. It does not -- it
    // writes the matches to an OUTPUT and never touches the source. Here we copy the
    // evens out; nums still has all six elements.
    std::vector<int> evens;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
        [](int x){ return x % 2 == 0; });
    std::cout << "after copy_if, source size: " << nums.size() << " (unchanged)\n";   // 6
    std::cout << "evens copied out: ";
    for (int x : evens) std::cout << x << ' ';
    std::cout << '\n';   // 4 2 8

    // FIX: to shrink the SOURCE in place, use the erase-remove idiom. remove_if
    // shifts the keepers to the front and returns the new logical end; erase drops
    // the leftover tail.
    nums.erase(std::remove_if(nums.begin(), nums.end(),
                              [](int x){ return x % 2 != 0; }),   // remove the odds
               nums.end());
    std::cout << "after erase-remove, source: ";
    for (int x : nums) std::cout << x << ' ';
    std::cout << '\n';   // 4 2 8
    return 0;
}
