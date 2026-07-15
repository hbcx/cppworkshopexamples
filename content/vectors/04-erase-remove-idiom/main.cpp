#include <algorithm>
#include <iostream>
#include <vector>

static void print(const std::vector<int>& v, const char* label) {
    std::cout << label;
    for (int x : v) {
        std::cout << ' ' << x;
    }
    std::cout << "   (size=" << v.size() << ")\n";
}

int main() {
    std::vector<int> v{1, 2, 3, 2, 4, 2, 5};
    print(v, "before:         ");

    // std::remove shifts the elements that are NOT 2 to the front and returns an
    // iterator to the new logical end. It does not, and cannot, change the size:
    // an algorithm only sees iterators, not the container.
    auto new_end = std::remove(v.begin(), v.end(), 2);

    // Size is unchanged here; the tail past new_end is unspecified leftover, so
    // we print only the size rather than misleading contents.
    std::cout << "after remove(2): size=" << v.size()
              << " (unchanged; kept elements moved to the front)\n";

    // erase is what actually removes the tail and shrinks the vector.
    v.erase(new_end, v.end());
    print(v, "after erase:    ");
    return 0;
}
