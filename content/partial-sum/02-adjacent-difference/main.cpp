#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 3, 6, 10, 15};

    // adjacent_difference writes the difference of each element and the one before
    // it: out[0] is a COPY of in[0], then out[i] = in[i] - in[i-1]. The output has
    // the SAME length as the input (n elements, not n-1).
    std::vector<int> diffs;
    std::adjacent_difference(v.begin(), v.end(), std::back_inserter(diffs));

    std::cout << "differences: ";
    for (int x : diffs) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
