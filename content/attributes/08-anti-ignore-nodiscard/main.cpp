#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};

    // ANTI-PATTERN: calling a [[nodiscard]] function for a side effect it does not
    // have. std::vector::empty() is [[nodiscard]] since C++20 -- it ASKS whether
    // the vector is empty, it does not make it empty. As a bare statement it does
    // nothing, and the attribute flags exactly this mistake:
    //
    //     v.empty();   // warning: ignoring return value of ... [[nodiscard]]
    //                  // (a common typo for v.clear())
    //
    // Under -Werror that warning stops the build, catching the bug at compile time.

    // FIX (the intent was to empty it): call the function with the side effect.
    v.clear();
    std::cout << "after clear(), size = " << v.size() << "\n";

    // FIX (the intent was to test it): use the result.
    std::vector<int> w{9};
    if (!w.empty())
        std::cout << "w is not empty, front = " << w.front() << "\n";
    return 0;
}
