#include <iostream>
#include <set>

// A CORRECT strict weak ordering: irreflexive (never a < a) and consistent.
struct Good {
    bool operator()(int a, int b) const { return a < b; }
};

// An over-coarse but still valid ordering: it compares by the tens digit, so 11
// and 15 are "equivalent". It is a legal comparator, but it changes IDENTITY --
// the set treats equivalent values as the same key and keeps only the first.
struct ByTens {
    bool operator()(int a, int b) const { return a / 10 < b / 10; }
};

int main() {
    // Anti-pattern (shown, NOT run): a comparator that is not irreflexive, e.g.
    // returning a <= b. Then comp(x, x) is true, so the set believes x < x, which
    // violates the strict weak ordering the red-black tree depends on -- the result
    // is undefined behaviour (lost elements, failed finds, or a crash):
    //   struct Bad { bool operator()(int a, int b) const { return a <= b; } };
    //   std::set<int, Bad> broken;   // do not do this

    std::set<int, Good> ok{3, 1, 2, 1};
    std::cout << "correct comparator:     size=" << ok.size() << " {";
    for (int x : ok) {
        std::cout << ' ' << x;
    }
    std::cout << " }\n";

    // A subtler mistake: a valid comparator whose granularity is wrong silently
    // merges distinct values, because the comparator defines identity.
    std::set<int, ByTens> coarse{11, 15, 23, 42, 48};
    std::cout << "over-coarse comparator: size=" << coarse.size() << " {";
    for (int x : coarse) {
        std::cout << ' ' << x;
    }
    std::cout << " }  <- 15 and 48 dropped as duplicates of 11 and 42\n";
    return 0;
}
