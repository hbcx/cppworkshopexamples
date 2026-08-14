#include <list>
#include <forward_list>
#include <algorithm>
#include <iostream>

int main() {
    std::list<int> data{1, 2, 3, 4, 5};

    // BAD-ish: std::reverse works on a std::list (its iterators are bidirectional),
    // but it reverses by SWAPPING element values between nodes -- needless value
    // moves on a container whose nodes are meant to stay put.
    std::list<int> viaAlgo = data;
    std::reverse(viaAlgo.begin(), viaAlgo.end());

    // GOOD: list has a member reverse that RELINKS the nodes -- it flips the
    // next/prev pointers, moving no values, in one pass.
    std::list<int> viaMember = data;
    viaMember.reverse();

    // For std::forward_list there is no choice: its iterators are only FORWARD, so
    // std::reverse would not even compile on it -- the member is the only way.
    //   std::reverse(fl.begin(), fl.end());   // ill-formed: needs bidirectional
    std::forward_list<int> fl{1, 2, 3, 4, 5};
    fl.reverse();

    std::cout << "std::reverse on list:   ";
    for (int x : viaAlgo) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "list::reverse (member): ";
    for (int x : viaMember) std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "forward_list::reverse:  ";
    for (int x : fl) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
