#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // BAD: default-construct the elements, then overwrite them all with fill. The
    // vector first value-initializes six ints to 0, then fill sets them to 7 --
    // two passes over the data to end up where one pass would.
    std::vector<int> bad(6);
    std::fill(bad.begin(), bad.end(), 7);

    // GOOD: the fill constructor sets every element to the value as the container
    // is built -- one pass, and the intent is right on the declaration line.
    std::vector<int> good(6, 7);

    std::cout << "bad == good? " << std::boolalpha << (bad == good) << '\n';

    // The same applies to growing: resize(n, value) fills the NEW elements, so
    // there is no need to resize and then fill the tail by hand.
    std::vector<int> g{1, 2};
    g.resize(5, 9);   // appends 9 9 9
    std::cout << "resized: ";
    for (int x : g) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
