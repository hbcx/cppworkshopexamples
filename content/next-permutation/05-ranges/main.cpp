#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct Card {
    std::string label;
    int rank;
};

int main() {
    std::vector<int> v{1, 2, 3};

    // ranges::next_permutation takes the range directly and returns a struct whose
    // .found is the bool -- true if it produced a next permutation.
    auto r = std::ranges::next_permutation(v);
    std::cout << "next of 1 2 3: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << "(found = " << std::boolalpha << r.found << ")\n";

    // ranges::is_permutation takes both ranges, a comparator, and a PROJECTION per
    // range, so two ranges of structs are compared by a field. Same ranks here,
    // different order and different labels.
    std::vector<Card> hand{{"A", 1}, {"B", 2}, {"C", 3}};
    std::vector<Card> other{{"X", 3}, {"Y", 1}, {"Z", 2}};
    bool same =
        std::ranges::is_permutation(hand, other, {}, &Card::rank, &Card::rank);
    std::cout << "same ranks in any order? " << same << '\n';
    return 0;
}
