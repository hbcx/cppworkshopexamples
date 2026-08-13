#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3};   // already sorted = the FIRST permutation

    // To visit EVERY permutation, start from the sorted (smallest) one and loop
    // with a do-while: process the current arrangement, THEN step to the next,
    // until next_permutation wraps and returns false. This yields all 3! = 6.
    int count = 0;
    do {
        for (int x : v) std::cout << x;
        std::cout << ' ';
        ++count;
    } while (std::next_permutation(v.begin(), v.end()));

    std::cout << "\ntotal: " << count << " permutations\n";
    return 0;
}
