#include <list>
#include <algorithm>
#include <iostream>

int main() {
    std::list<int> data{2, 2, 5, 5, 5, 1, 1, 4};

    // BAD-ish: std::unique + erase works on a list, but std::unique compacts by
    // moving element VALUES between nodes, which defeats the stable-node property
    // a list exists for, and ignores the member function built for the job.
    std::list<int> viaAlgo = data;
    viaAlgo.erase(std::unique(viaAlgo.begin(), viaAlgo.end()), viaAlgo.end());

    // GOOD: list has a member unique that UNLINKS the duplicate nodes in place --
    // no values move, the surviving nodes keep their addresses, and it resizes the
    // list in one call. It too collapses consecutive duplicates only.
    std::list<int> viaMember = data;
    viaMember.unique();

    std::cout << "via std::unique:  ";
    for (int x : viaAlgo) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "via list::unique: ";
    for (int x : viaMember) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
