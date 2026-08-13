#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3};

    // BAD: growing the SAME vector inside the callback. push_back can reallocate,
    // which invalidates the begin/end iterators for_each is walking, so the loop
    // then steps over freed memory -- undefined behavior. Also, appending while
    // iterating can feed the new elements back into the loop. Shown as a comment:
    //   std::for_each(v.begin(), v.end(), [&v](int x) { v.push_back(x * 10); });

    // GOOD: produce the new elements into a SEPARATE container, then append once.
    // for_each never resizes the range it was given, and neither should its
    // callback.
    std::vector<int> extra;
    std::for_each(v.begin(), v.end(), [&extra](int x) { extra.push_back(x * 10); });
    v.insert(v.end(), extra.begin(), extra.end());

    std::cout << "result: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
