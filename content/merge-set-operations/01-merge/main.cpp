#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 3, 5, 7};   // sorted
    std::vector<int> b{2, 3, 6};      // sorted

    // merge combines two SORTED ranges into one sorted output. It keeps EVERY
    // element from both, including duplicates -- the 3 that is in each appears
    // twice here.
    std::vector<int> out;
    std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(out));

    std::cout << "merged: ";
    for (int x : out) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
