#include <iostream>
#include <vector>
#include <iterator>   // std::begin, std::end, std::distance

// ANTI-PATTERN (shown in a comment -- it would NOT compile for a raw array):
//
//   template <typename Range>
//   void describe(const Range& r) {
//       std::cout << "first = " << *r.begin()
//                 << ", count = " << r.size() << "\n";   // members
//   }
//   int arr[] = {1, 2, 3, 4};
//   describe(arr);   // ERROR: request for member 'begin' in 'r' (arr is int[4])
//
// The member form silently locks the template to class types. A raw array is
// rejected, and so is any library range that offers only free begin/end.

// FIX: go through std::begin/std::end (and std::distance for the count), which
// are specialised for arrays as well as containers.
template <typename Range>
void describe(const Range& r) {
    auto b = std::begin(r);
    auto e = std::end(r);
    std::cout << "first = " << *b
              << ", count = " << std::distance(b, e) << "\n";
}

int main() {
    std::vector<int> v = {7, 8, 9};
    int arr[] = {1, 2, 3, 4};

    describe(v);     // works
    describe(arr);   // works too -- the whole point of the free functions
    return 0;
}
