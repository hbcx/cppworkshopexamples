#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> scores{72, 45, 88, 30, 91, 55, 60};
    int pass = 60;

    // partition_copy reads the input once and writes to TWO outputs: elements that
    // satisfy the predicate go to the first, the rest to the second. The source is
    // left unchanged, and each group keeps the input order.
    std::vector<int> passed, failed;
    std::partition_copy(scores.begin(), scores.end(),
                        std::back_inserter(passed),
                        std::back_inserter(failed),
                        [pass](int s) { return s >= pass; });

    std::cout << "passed: ";
    for (int s : passed) std::cout << s << ' ';
    std::cout << "\nfailed: ";
    for (int s : failed) std::cout << s << ' ';
    std::cout << '\n';
    return 0;
}
