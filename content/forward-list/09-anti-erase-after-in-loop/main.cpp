#include <forward_list>
#include <iostream>

static void print(const std::forward_list<int>& fl, const char* label) {
    std::cout << label;
    for (int x : fl)
        std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    const int target = 7;

    // ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
    //
    //   for (auto prev = fl.before_begin(), it = fl.begin(); it != fl.end();
    //        ++prev, ++it) {
    //       if (*it == target)
    //           fl.erase_after(prev);   // erases *it, INVALIDATING it
    //   }
    //
    // On the step that erased, `it` now dangles, so the ++it that follows is UB;
    // and even if it survived, advancing over the freed slot would skip the node
    // that slid into it, missing adjacent matches.

    // FIX: hold a trailing iterator, and advance it ONLY when nothing was erased.
    // erase_after returns the node after the removed one, which becomes the new
    // current position while `prev` stays put.
    std::forward_list<int> fl{7, 7, 1, 7, 2, 7, 7, 3};
    auto prev = fl.before_begin();
    auto it = fl.begin();
    while (it != fl.end()) {
        if (*it == target) {
            it = fl.erase_after(prev);   // drop *it; prev is unchanged
        } else {
            prev = it;
            ++it;
        }
    }
    print(fl, "after trailing-iterator erase: ");   // 1 2 3

    // When the whole test fits a predicate, the member remove_if does all of
    // this in one correct pass.
    std::forward_list<int> copy{7, 7, 1, 7, 2, 7, 7, 3};
    copy.remove_if([](int x) { return x == target; });
    print(copy, "after remove_if:               ");   // 1 2 3
    return 0;
}
