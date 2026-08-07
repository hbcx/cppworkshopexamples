#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

// CORRECT: cover the weaker iterators too. random_access gets the O(1) path;
// everything from forward down falls back to the counting overload written for
// input_iterator_tag, routed there by tag inheritance.
template <class It>
std::size_t countSteps(It first, It last, std::random_access_iterator_tag) {
    std::cout << "random-access: direct subtraction\n";
    return static_cast<std::size_t>(last - first);
}
template <class It>
std::size_t countSteps(It first, It last, std::input_iterator_tag) {
    std::cout << "input/forward/bidirectional: counting loop\n";
    std::size_t n = 0;
    for (; first != last; ++first) ++n;
    return n;
}
template <class It>
std::size_t countSteps(It first, It last) {
    return countSteps(first, last, typename std::iterator_traits<It>::iterator_category{});
}

int main() {
    std::vector<int> v = {1, 2, 3, 4};
    std::cout << "vector distance = " << countSteps(v.begin(), v.end()) << "\n";

    std::list<int> l = {1, 2, 3};
    std::cout << "list distance   = " << countSteps(l.begin(), l.end()) << "\n";

    // THE ANTI-PATTERN: provide ONLY the random_access overload and drop the
    // input_iterator_tag fallback. Then countSteps(l.begin(), l.end()) fails to
    // compile -- a list's bidirectional tag does not convert to
    // random_access_iterator_tag, so there is NO matching overload, and the
    // error points deep inside the dispatcher, not at the call. Always provide
    // an overload for the weakest category you support (here input_iterator_tag).
    return 0;
}
