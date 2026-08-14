#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> source{1, 2, 3, 4, 5};

    // std::reverse_copy writes the range reversed into a NEW destination and
    // leaves the source untouched. Like the other _copy algorithms it writes
    // through an output iterator, so use a back_inserter or a pre-sized target.
    std::vector<int> reversed;
    std::reverse_copy(source.begin(), source.end(), std::back_inserter(reversed));
    print("source:        ", source);     // unchanged
    print("reverse_copy:  ", reversed);    // 5 4 3 2 1

    // std::rotate_copy writes a rotated copy: starting from the given middle, it
    // emits middle..end then begin..middle, so that element becomes first in the
    // output. The source is again left as it was.
    std::vector<int> rotated;
    auto middle = source.begin() + 2;      // start the copy at value 3
    std::rotate_copy(source.begin(), middle, source.end(),
                     std::back_inserter(rotated));
    print("rotate_copy@3: ", rotated);     // 3 4 5 1 2

    // These are the non-mutating forms of reverse and rotate: reach for them when
    // you need the transformed sequence but must keep the original intact.
    return 0;
}
