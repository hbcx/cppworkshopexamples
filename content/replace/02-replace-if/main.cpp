#include <algorithm>
#include <iostream>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> data{5, -3, 12, -8, 0, 20, -1};
    print("before:      ", data);

    // std::replace_if(first, last, pred, new) overwrites every element for which
    // pred returns true. Here: floor all negatives to 0 -- a condition, not a
    // single value, so replace_if fits where replace cannot.
    std::replace_if(data.begin(), data.end(),
                    [](int x) { return x < 0; }, 0);
    print("negatives->0:", data);

    // The predicate can be any test: cap everything above a threshold at that
    // threshold (a simple upper clamp over the whole range).
    std::replace_if(data.begin(), data.end(),
                    [](int x) { return x > 10; }, 10);
    print("cap at 10:   ", data);
    return 0;
}
