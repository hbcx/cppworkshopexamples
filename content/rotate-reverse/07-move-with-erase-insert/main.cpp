#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

int main() {
    std::vector<std::string> items{"a", "b", "c", "D", "e"};
    const std::size_t from = 3, to = 1;   // move "D" from index 3 to index 1

    // BAD: erase then insert to reposition one element. erase shifts the tail down,
    // insert shifts a tail back up -- two O(n) passes -- and erase INVALIDATES every
    // iterator from the erase point on, so the value must be copied out first to
    // survive across the two steps.
    std::vector<std::string> bad = items;
    std::string moved = bad[from];        // copy out; erase would invalidate a ref
    bad.erase(bad.begin() + from);
    bad.insert(bad.begin() + to, moved);

    // GOOD: std::rotate repositions the element in a single in-place pass over just
    // the affected sub-range -- no copy-out, no second shift, no reallocation.
    std::vector<std::string> good = items;
    std::rotate(good.begin() + to, good.begin() + from, good.begin() + from + 1);

    std::cout << "erase+insert: ";
    for (const auto& s : bad) std::cout << s << ' ';
    std::cout << '\n';
    std::cout << "std::rotate:  ";
    for (const auto& s : good) std::cout << s << ' ';
    std::cout << '\n';
    return 0;
}
