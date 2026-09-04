#include <forward_list>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    // ANTI-PATTERN: using std::prev or a NEGATIVE std::advance on a forward-only
    // iterator. Both require a BIDIRECTIONAL iterator. std::forward_list,
    // std::unordered_set / unordered_map and istream_iterator provide only FORWARD
    // iterators, so these do not compile / are undefined:
    //
    //   std::forward_list<int> fl{1, 2, 3};
    //   auto it = fl.end();
    //   auto last = std::prev(it);   // does NOT compile: prev needs bidirectional
    //   std::advance(it, -1);        // negative step also needs bidirectional
    //
    // The iterator category is a compile-time contract; know it before stepping back.

    // FIX 1: on a forward-only container there is no cheap "previous". Track a
    // trailing iterator as you walk forward.
    std::forward_list<int> fl{1, 2, 3, 4};
    auto lastIt = fl.begin();
    for (auto it = fl.begin(); it != fl.end(); ++it) lastIt = it;   // ends on the last node
    std::cout << "last of forward_list = " << *lastIt << "\n";      // 4

    // FIX 2: if you need to step backward often, use a bidirectional container
    // (std::list) or a random-access one (std::vector), where prev / advance(-n) work.
    std::vector<int> v{1, 2, 3, 4};
    std::cout << "prev(v.end()) = " << *std::prev(v.end()) << "\n"; // 4
    return 0;
}
