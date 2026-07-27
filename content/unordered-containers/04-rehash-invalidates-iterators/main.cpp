// A rehash rebuilds the bucket array, so you might expect it to wreck everything
// pointing into the container. It does not. The standard is precise here:
//
//   a rehash invalidates ITERATORS,
//   but NOT pointers or references to the elements themselves.
//
// The elements live in nodes that are not moved by a rehash -- only the bucket
// array that indexes them is rebuilt. So a pointer or reference taken before a
// rehash still names the same element afterwards; an iterator taken before it is
// dangling and must not be used.

#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> scores;
    scores["alice"] = 100;

    // A pointer and a reference to alice's value, taken now.
    int* p = &scores["alice"];
    int& r = scores["alice"];

    // Force many rehashes by growing the table from almost nothing.
    for (int i = 0; i < 100000; ++i) {
        scores[std::to_string(i)] = i;
    }

    // An ITERATOR taken before that loop would now be invalid:
    //
    //   std::unordered_map<std::string,int>::iterator it = scores.find("alice");
    //   // ... after the rehash, *it is undefined behaviour -- do NOT do this.
    //
    // We never touch such an iterator. p and r, however, are still valid, so we
    // read and write alice's value straight through them.
    *p += 1;
    r += 1;

    std::cout << "alice, reached via the pre-rehash pointer/reference: "
              << scores["alice"] << "\n";
    std::cout << "size after growth: " << scores.size() << "\n";
    return 0;
}
