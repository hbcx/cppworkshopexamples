#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Does range a come before range b in dictionary order?
static bool before(const std::vector<int>& a, const std::vector<int>& b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

int main() {
    std::cout << std::boolalpha;

    // std::lexicographical_compare orders two ranges the way a dictionary orders
    // words: compare element by element; at the first difference the smaller
    // element decides. It returns true if the first range is LESS than the second.
    std::cout << "{1,2,3} < {1,2,4}? " << before({1, 2, 3}, {1, 2, 4}) << '\n';
    std::cout << "{1,2,4} < {1,2,3}? " << before({1, 2, 4}, {1, 2, 3}) << '\n';

    // If one range is a prefix of the other, the SHORTER one is less -- exactly
    // like "car" comes before "cart" in a dictionary.
    std::cout << "{1,2} < {1,2,3}?   " << before({1, 2}, {1, 2, 3}) << '\n';

    // Equal ranges are NOT less than each other, so it returns false both ways --
    // this is an ordering, not an equality test (see the anti-patterns).
    std::cout << "{1,2,3} < {1,2,3}? " << before({1, 2, 3}, {1, 2, 3}) << '\n';

    // It is the comparison behind sorting sequences of sequences: sort a list of
    // words and each is compared to the next with lexicographical order.
    std::vector<std::string> words{"delta", "alpha", "charlie", "bravo"};
    std::sort(words.begin(), words.end());
    std::cout << "sorted:";
    for (const auto& w : words) std::cout << ' ' << w;
    std::cout << '\n';
    return 0;
}
