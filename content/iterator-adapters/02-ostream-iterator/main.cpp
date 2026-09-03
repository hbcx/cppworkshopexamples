#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4};

    // ostream_iterator writes each assigned value to a stream, with an optional
    // delimiter after it. std::copy into one prints the whole range.
    std::cout << "space delim: ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";   // 1 2 3 4

    // The delimiter can be any string, and any type the stream can print works.
    std::cout << "csv: ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ","));
    std::cout << "\n";   // 1,2,3,4,   <- note the TRAILING comma

    // NOTE the trailing delimiter: ostream_iterator puts the separator AFTER each
    // element, not strictly between them. For a clean "a, b, c" you special-case
    // the first element (or, in C++20, print a range view with a separator).
    return 0;
}
