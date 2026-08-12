#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> ids{101, 102, 103};

    // BAD (shown in a comment -- dereferencing end() is undefined behavior):
    //
    //   auto it = std::find(ids.begin(), ids.end(), 999);
    //   int value = *it;   // 999 is absent, so it == end(); *it reads past the vector -- UB
    //
    // find ALWAYS returns an iterator. When the value is absent that iterator is
    // end(), which does not point at an element -- dereferencing it, or advancing
    // from it, is undefined behavior: it may read garbage, corrupt memory, or crash.

    // GOOD: compare against end() before touching the iterator.
    auto it = std::find(ids.begin(), ids.end(), 999);
    if (it != ids.end())
        std::cout << "found: " << *it << '\n';
    else
        std::cout << "999 not found -- did not dereference\n";
    return 0;
}
