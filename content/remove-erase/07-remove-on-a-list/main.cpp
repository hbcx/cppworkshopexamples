#include <list>
#include <algorithm>
#include <iostream>

int main() {
    std::list<int> data{3, 1, 4, 1, 5, 1, 9};

    // BAD-ish: std::remove + erase works on a list, but std::remove compacts by
    // moving element VALUES around, which throws away the one thing a list gives
    // you -- its nodes never move. For a node-based container this copies values
    // needlessly and ignores the member function built for the job.
    std::list<int> viaAlgo = data;
    viaAlgo.erase(std::remove(viaAlgo.begin(), viaAlgo.end(), 1), viaAlgo.end());

    // GOOD: list has a member remove that UNLINKS the matching nodes in place.
    // No values are moved, the surviving nodes keep their addresses, and it
    // resizes the list in one call. remove_if is the predicate form.
    std::list<int> viaMember = data;
    viaMember.remove(1);

    std::cout << "via std::remove:  ";
    for (int x : viaAlgo) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "via list::remove: ";
    for (int x : viaMember) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
