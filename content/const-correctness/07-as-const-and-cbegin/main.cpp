#include <iostream>
#include <vector>
#include <utility>      // std::as_const

// Sometimes you have a mutable object but want to force the const path -- to
// pick a const overload, get a const_iterator, or make sure code you hand it to
// cannot modify it. std::as_const (C++17) gives a const view of an lvalue with
// no copy, and cbegin/cend hand out const_iterators.

int main() {
    std::vector<int> v{1, 2, 3};

    // cbegin/cend yield const_iterators even though v is not const.
    std::cout << "via cbegin: ";
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        // *it = 0;   // error: a const_iterator is read-only
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // std::as_const gives a const view of v: overload resolution now takes the
    // const paths, and anything you pass this view to cannot write through it.
    const std::vector<int>& cv = std::as_const(v);
    std::cout << "as_const size  = " << cv.size() << "\n";
    std::cout << "as_const front = " << cv.front() << "\n";

    // v itself is still mutable; as_const only changed the view, not the object.
    v[0] = 99;
    std::cout << "after v[0]=99, view front = " << cv.front() << "\n";  // 99
    return 0;
}
