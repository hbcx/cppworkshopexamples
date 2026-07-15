#include <iostream>
#include <set>

int main() {
    std::set<int> s;

    // Insert in a jumbled order, with the value 1 repeated three times.
    for (int x : {5, 1, 3, 1, 4, 1, 2}) {
        s.insert(x);
    }

    std::cout << "elements (sorted, unique):";
    for (int x : s) {
        std::cout << ' ' << x;
    }
    std::cout << "\nsize = " << s.size() << " (the three 1s collapsed to one)\n";
    return 0;
}
