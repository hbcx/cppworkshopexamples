#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

int main() {
    std::vector<int> src{1, 2, 3};

    // back_inserter turns push_back into an OUTPUT iterator: each assignment
    // appends, so the destination grows on its own -- no pre-sizing.
    std::vector<int> dst;
    std::copy(src.begin(), src.end(), std::back_inserter(dst));
    std::cout << "back_inserter -> vector:";
    for (int x : dst) std::cout << " " << x;
    std::cout << "\n";   // 1 2 3

    // front_inserter uses push_front (containers that have it: deque, list). It
    // REVERSES the order, because every element is pushed to the front.
    std::deque<int> dq;
    std::copy(src.begin(), src.end(), std::front_inserter(dq));
    std::cout << "front_inserter -> deque:";
    for (int x : dq) std::cout << " " << x;
    std::cout << "\n";   // 3 2 1

    // inserter inserts at a given position -- the general one, and the only one
    // that works for associative containers (which have no push_back).
    std::set<int> s{10, 20};
    std::copy(src.begin(), src.end(), std::inserter(s, s.end()));
    std::cout << "inserter -> set:";
    for (int x : s) std::cout << " " << x;
    std::cout << "\n";   // 1 2 3 10 20  (the set orders them)
    return 0;
}
