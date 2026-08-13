#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // Small n is fine: enumerate all 4! = 24 permutations of four elements.
    std::vector<int> small{1, 2, 3, 4};
    int seen = 0;
    do { ++seen; } while (std::next_permutation(small.begin(), small.end()));
    std::cout << "4 elements: " << seen << " permutations enumerated\n";

    // But the count is n!, which explodes. Enumerating all permutations is only
    // feasible for TINY n -- these are how many next_permutation would have to visit:
    unsigned long long fact = 1;
    for (int n = 1; n <= 20; ++n) {
        fact *= static_cast<unsigned long long>(n);
        if (n == 10 || n == 13 || n == 15 || n == 20)
            std::cout << n << "! = " << fact << '\n';
    }

    // BAD: brute-forcing, say, the best route over 15 cities with next_permutation
    // -- 15! is over a trillion arrangements, hopeless to visit one by one.
    // GOOD: beyond ~10 elements use a smarter method (dynamic programming, branch
    // and bound, a heuristic), not exhaustive permutation.
    return 0;
}
