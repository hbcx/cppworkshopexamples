#include <iostream>
#include <vector>
#include <iterator>   // std::begin, std::end
#include <cstddef>    // std::size_t

namespace text {
    // A range type from some library: traversal offered ONLY through free
    // begin/end in its namespace, with no member begin().
    struct Line {
        const char* s;
        std::size_t n;
    };
    const char* begin(const Line& l) { return l.s; }
    const char* end(const Line& l)   { return l.s + l.n; }
}

// ANTI-PATTERN (shown in a comment -- it would NOT compile for text::Line):
//
//   template <typename Range>
//   std::ptrdiff_t length(const Range& r) {
//       return std::end(r) - std::begin(r);   // QUALIFIED std::begin
//   }
//   text::Line line{"hello", 5};
//   length(line);   // ERROR: std::begin needs r.begin(); text::begin is ignored
//
// The qualified name freezes the lookup: the compiler never considers
// text::begin, so a range with only free begin/end is rejected. std::begin does
// not fall back to a namespace-scope begin.

// FIX: unqualified call after a using-declaration, so ADL is back in play and
// finds text::begin for Line while still using std::begin for the vector.
template <typename Range>
std::ptrdiff_t length(const Range& r) {
    using std::begin;
    using std::end;
    return end(r) - begin(r);
}

int main() {
    std::vector<char> v = {'a', 'b', 'c'};
    text::Line line = {"hello", 5};

    std::cout << "length(vector) = " << length(v) << "\n";
    std::cout << "length(Line)   = " << length(line) << "\n";
    return 0;
}
