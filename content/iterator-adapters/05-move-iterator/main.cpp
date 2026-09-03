#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> src{"alpha", "beta", "gamma"};

    // make_move_iterator turns each dereference into an rvalue, so an algorithm
    // MOVES elements instead of copying them. Here: move src's strings into dst.
    std::vector<std::string> dst;
    std::copy(std::make_move_iterator(src.begin()),
              std::make_move_iterator(src.end()),
              std::back_inserter(dst));

    std::cout << "dst:";
    for (const auto& s : dst) std::cout << " " << s;
    std::cout << "\n";   // alpha beta gamma

    // The source strings were moved from: still valid, but unspecified (in this
    // library, empty). We check only their SIZE, to stay deterministic.
    std::cout << "src sizes after move:";
    for (const auto& s : src) std::cout << " " << s.size();
    std::cout << "\n";   // 0 0 0

    // A common shortcut: insert a range with move_iterators directly.
    std::vector<std::string> more{"x", "y"};
    dst.insert(dst.end(),
               std::make_move_iterator(more.begin()),
               std::make_move_iterator(more.end()));
    std::cout << "dst size now = " << dst.size() << "\n";   // 5
    return 0;
}
