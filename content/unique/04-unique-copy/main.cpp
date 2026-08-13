#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> readings{4, 4, 4, 7, 7, 2, 2, 2, 9};

    // unique_copy writes the collapsed (consecutive-deduplicated) result to an
    // OUTPUT and leaves the source untouched -- no erase, no mutation. Collect
    // into a new container through a back_inserter.
    std::vector<int> collapsed;
    std::unique_copy(readings.begin(), readings.end(),
                     std::back_inserter(collapsed));

    std::cout << "source size still " << readings.size() << '\n';   // 9
    std::cout << "collapsed: ";
    for (int x : collapsed) std::cout << x << ' ';
    std::cout << '\n';

    // The output can be a stream, so unique_copy prints runs collapsed in one
    // pass with no temporary container.
    std::cout << "to stream: ";
    std::unique_copy(readings.begin(), readings.end(),
                     std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    return 0;
}
