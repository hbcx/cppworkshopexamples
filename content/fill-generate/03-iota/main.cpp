#include <numeric>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> ids(6);

    // iota fills a range with consecutive increasing values starting from a given
    // one: here 100, 101, 102, ... It is the named tool for a run of numbers.
    std::iota(ids.begin(), ids.end(), 100);

    std::cout << "sequential ids: ";
    for (int x : ids) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
