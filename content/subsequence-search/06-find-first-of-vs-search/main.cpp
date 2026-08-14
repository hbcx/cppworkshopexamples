#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

int main() {
    std::string haystack = "xxabcxx";
    std::string set = "cba";   // the letters c, b, a -- as a SET of candidates

    // ANTI-PATTERN: reaching for std::search when you mean "any one of these".
    // search treats "cba" as an ORDERED block and looks for c, then b, then a in
    // a row. The haystack contains "abc", not "cba", so search finds nothing.
    auto s = std::search(haystack.begin(), haystack.end(), set.begin(), set.end());
    std::cout << "search for block \"cba\": "
              << (s == haystack.end() ? "not found" : "found") << '\n';

    // RIGHT TOOL: std::find_first_of treats the second range as a SET and matches
    // the first element that is any one of c, b, a -- the 'a' at index 2.
    auto f = std::find_first_of(haystack.begin(), haystack.end(),
                                set.begin(), set.end());
    std::cout << "find_first_of any of {c,b,a}: '" << *f << "' at index "
              << std::distance(haystack.begin(), f) << '\n';

    // The two answer different questions. Use search for an ordered subsequence,
    // find_first_of for "the first element belonging to a set". Swapping them
    // compiles fine and silently gives the wrong answer.
    return 0;
}
