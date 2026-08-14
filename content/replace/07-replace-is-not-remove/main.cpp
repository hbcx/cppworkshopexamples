#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label << " (size " << v.size() << "):";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> data{5, 0, 7, 0, 9};

    // ANTI-PATTERN: expecting replace to DELETE the 0s. replace substitutes
    // one-for-one, so the size never changes -- turning 0 into something is not
    // the same as removing it. There is no "replace with nothing".
    std::vector<int> substituted = data;
    std::replace(substituted.begin(), substituted.end(), 0, -1);
    print("replace 0 -> -1", substituted);   // still 5 elements

    // FIX (drop in place): the erase-remove idiom actually shortens the range.
    std::vector<int> shrunk = data;
    shrunk.erase(std::remove(shrunk.begin(), shrunk.end(), 0), shrunk.end());
    print("erase-remove 0 ", shrunk);         // now 3 elements

    // FIX (drop into a copy): remove_copy writes everything EXCEPT the matches,
    // giving a shorter output while the source stays intact.
    std::vector<int> filtered;
    std::remove_copy(data.begin(), data.end(), std::back_inserter(filtered), 0);
    print("remove_copy 0  ", filtered);

    // Rule: replace = substitute (same length); remove = drop (shorter).
    return 0;
}
