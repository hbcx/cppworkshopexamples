#include <iostream>
#include <vector>
#include <iterator>   // std::begin, std::end

namespace geo {
    // A small range type that exposes traversal ONLY through free begin/end in
    // its own namespace -- the way many library range types are written.
    struct Triangle {
        int sides[3];
    };
    const int* begin(const Triangle& t) { return t.sides; }
    const int* end(const Triangle& t)   { return t.sides + 3; }
}

// The customization-point idiom: bring std::begin/std::end into scope, then
// call begin/end UNQUALIFIED. Now overload resolution sees std::begin (for the
// vector and the array) AND, via ADL, geo::begin (for the Triangle).
template <typename Range>
long total(const Range& r) {
    using std::begin;
    using std::end;
    long sum = 0;
    for (auto it = begin(r); it != end(r); ++it)
        sum += *it;
    return sum;
}

int main() {
    std::vector<int> v = {2, 4, 6};    // begin found via std::begin
    int arr[] = {1, 1, 1};             // begin found via std::begin (array overload)
    geo::Triangle t = { {3, 4, 5} };   // begin found via ADL -> geo::begin

    std::cout << "total(vector)   = " << total(v) << "\n";
    std::cout << "total(array)    = " << total(arr) << "\n";
    std::cout << "total(Triangle) = " << total(t) << "\n";
    return 0;
}
