// Inserting at either end of a deque keeps references and pointers to existing
// elements valid (the objects never move), but invalidates all iterators. This
// is the opposite of what a vector reallocation does to references.

#include <deque>
#include <iostream>

int main() {
    std::deque<int> d = {10, 20, 30};

    int& ref = d[1];          // reference to the element holding 20
    int* addr = &d[1];        // and its address

    d.push_back(40);          // grow at the back
    d.push_front(5);          // grow at the front (shifts every index up by 1)

    // The object holding 20 never moved: the reference and pointer still work.
    std::cout << std::boolalpha;
    std::cout << "reference still reads 20? " << (ref == 20) << '\n';
    std::cout << "same object, now at index 2? " << (&d[2] == addr) << '\n';

    // BAD (undefined -- described, not run):
    //   auto it = d.begin();   // taken before the pushes
    //   ... after push_back/push_front, *it is undefined: end insertion
    //   invalidates every iterator, even though references survived.

    // GOOD: re-fetch the iterator after the pushes.
    auto it = d.begin();
    std::cout << "re-fetched begin() reads " << *it << '\n';   // 5
}
