// Anti-pattern: mutating a container while iterating it.
//
// Erasing from a container during a loop invalidates iterators (and indices),
// and using them afterwards is either a silently wrong result or undefined
// behaviour. Two failures below: an index loop that erases skips the element
// that shifts into the freed slot (defined, but wrong), and -- described, not
// run -- the iterator form where erase invalidates the iterator and the next
// ++it is UB. The fix is to use erase's RETURN value, which is the valid
// iterator to the element after the removed one.

#include <iostream>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";
}

int main() {
    // Remove every even number. Correct answer: { 1, 3 }.

    // ---- BAD: index loop + erase. Defined, but SKIPS shifted elements. ----
    {
        std::vector<int> v{ 1, 2, 4, 3, 6, 8 };
        for (std::size_t i = 0; i < v.size(); ++i)
            if (v[i] % 2 == 0)
                v.erase(v.begin() + static_cast<std::ptrdiff_t>(i)); // shifts left; ++i skips one
        print("bad (index):", v); // leaves evens behind
    }

    // ---- BAD (undefined behaviour -- described, NOT run) ----
    //
    //   for (auto it = v.begin(); it != v.end(); ++it)
    //       if (*it % 2 == 0)
    //           v.erase(it);   // erase invalidates it; the next ++it is UB
    //
    // For a vector, erase invalidates every iterator at or after the erased
    // position, so continuing to use `it` (even to ++ it) is undefined.

    // ---- GOOD: use erase's return value, advance only when NOT erasing. ----
    {
        std::vector<int> v{ 1, 2, 4, 3, 6, 8 };
        for (auto it = v.begin(); it != v.end(); )
            if (*it % 2 == 0)
                it = v.erase(it); // erase returns the iterator to the next element
            else
                ++it;
        print("good:      ", v);
    }

    // The idiomatic one-liner for this specific task is erase-remove:
    //   v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());
    // (see the std::vector chapter's erase-remove example).
    return 0;
}
