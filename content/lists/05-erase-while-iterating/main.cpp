#include <iostream>
#include <list>

static void print(const std::list<int>& l, const char* label) {
    std::cout << label;
    for (int x : l) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    std::list<int> l{1, 2, 3, 4, 5, 6, 7, 8};

    // The broken loop (shown, NOT run): erase kills the iterator it is given,
    // and ++it then steps off a destroyed node.
    //   for (auto it = l.begin(); it != l.end(); ++it) {
    //       if (*it % 2 == 0) { l.erase(it); }   // 'it' is dead after this
    //   }

    // The fix: erase hands back the position that followed. Advance EITHER by
    // erase or by ++, never by both.
    for (auto it = l.begin(); it != l.end(); ) {
        if (*it % 2 == 0) {
            it = l.erase(it);      // returns the next element
        } else {
            ++it;
        }
    }
    print(l, "odd values kept: ");

    // Unlike a vector, only the erased node is affected -- an iterator to any
    // other element keeps working across the whole loop.
    std::list<int> other{10, 20, 30, 40};
    auto watched = ++other.begin();          // -> 20
    for (auto it = other.begin(); it != other.end(); ) {
        if (*it >= 30) {
            it = other.erase(it);
        } else {
            ++it;
        }
    }
    print(other, "below 30 kept:   ");
    std::cout << "iterator taken before the loop still reads: " << *watched << '\n';

    // The version to actually write: same result, no loop to get wrong.
    std::list<int> third{1, 2, 3, 4, 5, 6, 7, 8};
    third.remove_if([](int x) { return x % 2 == 0; });
    print(third, "remove_if:       ");
    return 0;
}
