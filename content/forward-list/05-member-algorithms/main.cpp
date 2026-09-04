#include <forward_list>
#include <iostream>

static void print(const std::forward_list<int>& fl, const char* label) {
    std::cout << label;
    for (int x : fl)
        std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::forward_list<int> fl{5, 3, 9, 3, 1, 5, 5};

    // std::sort(fl.begin(), fl.end()) would NOT compile: it needs random-access
    // iterators. The member sort relinks the nodes instead (stable merge sort).
    fl.sort();
    print(fl, "after sort:      ");   // 1 3 3 5 5 5 9

    // unique drops ADJACENT equal elements, so sort first to remove all dups.
    fl.unique();
    print(fl, "after unique:    ");   // 1 3 5 9

    // remove_if unlinks every node matching the predicate in one pass.
    fl.remove_if([](int x) { return x % 2 != 0 && x > 4; });  // drop odd values > 4
    print(fl, "after remove_if: ");   // 1 3

    // merge folds another SORTED list in by relinking; the source ends empty.
    std::forward_list<int> other{2, 4, 6};
    fl.merge(other);
    print(fl, "after merge:     ");   // 1 2 3 4 6
    std::cout << "other empty? " << std::boolalpha << other.empty() << '\n';

    // reverse flips the next pointers.
    fl.reverse();
    print(fl, "after reverse:   ");   // 6 4 3 2 1
    return 0;
}
