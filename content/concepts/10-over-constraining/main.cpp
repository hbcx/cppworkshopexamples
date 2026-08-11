#include <concepts>
#include <cstddef>
#include <iostream>
#include <list>
#include <vector>

// BAD: this concept demands operator[], but the function only iterates. It
// needlessly rejects perfectly good types like std::list.
template <class T>
concept Indexable = requires(T& c, std::size_t i) {
    c[i];
    c.begin();
    c.end();
};

template <Indexable T>
std::size_t countStrict(const T& c) {
    std::size_t n = 0;
    for (auto it = c.begin(); it != c.end(); ++it) ++n;   // only iterates!
    return n;
}

// GOOD: require only what the function actually uses -- begin() and end().
template <class T>
concept Iterable = requires(const T& c) {
    c.begin();
    c.end();
};

template <Iterable T>
std::size_t countLoose(const T& c) {
    std::size_t n = 0;
    for (auto it = c.begin(); it != c.end(); ++it) ++n;
    return n;
}

int main() {
    std::vector<int> v = {1, 2, 3};
    std::list<int>   l = {1, 2, 3, 4};

    std::cout << "countStrict(vector) = " << countStrict(v) << "\n";   // ok
    // countStrict(l);   // ERROR: std::list has no operator[], so it fails the
    //                   // Indexable concept -- even though counting never indexes.
    std::cout << "countLoose(vector)  = " << countLoose(v) << "\n";
    std::cout << "countLoose(list)    = " << countLoose(l) << "\n";    // now works
    std::cout << "done\n";
}
