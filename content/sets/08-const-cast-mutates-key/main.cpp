#include <iostream>
#include <set>
#include <string>

static void print(const std::set<std::string>& s, const char* label) {
    std::cout << label;
    for (const auto& x : s) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    std::set<std::string> names{"bob", "carol", "dave"};
    print(names, "before:            ");

    // Anti-pattern (shown, NOT run): the element is const for a reason, but people
    // reach for const_cast to "edit in place":
    //   auto it = names.find("bob");
    //   const_cast<std::string&>(*it) = "zzz";   // UB: "zzz" now sits in bob's
    //                                             // node, so the tree is unsorted
    // After that, find("zzz") can fail to locate an element that is physically in
    // the set, and later inserts may land in the wrong place.

    // Correct: erase the old value and insert the new one, so the tree re-sorts.
    names.erase("bob");
    names.insert("zzz");
    print(names, "after erase+insert:");
    std::cout << "find(zzz) works? "
              << (names.find("zzz") != names.end() ? "yes" : "no") << '\n';
    return 0;
}
