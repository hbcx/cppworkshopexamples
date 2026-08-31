// Anti-pattern: keeping an iterator across a push at either end.
//
// push_back / push_front invalidate every iterator into a deque, even though
// references to elements survive. A saved iterator used after a push is
// undefined -- re-fetch it, or track the position by reference or index.

#include <deque>
#include <iostream>

int main() {
    std::deque<int> d = {10, 20, 30};

    int& keptRef = d.front();     // a reference -- this WILL survive the push
    auto staleIt = d.begin();     // an iterator -- this will NOT
    (void)staleIt;

    d.push_back(40);              // invalidates all iterators

    // BAD (undefined -- described, not run):
    //   std::cout << *staleIt;   // staleIt was invalidated by push_back

    // GOOD: references survived, so keptRef is still fine.
    std::cout << "kept reference still reads: " << keptRef << '\n';   // 10

    // GOOD: re-fetch the iterator after the push.
    auto freshIt = d.begin();
    std::cout << "re-fetched begin() reads: " << *freshIt << '\n';    // 10

    std::cout << "size is now " << d.size() << '\n';
}
