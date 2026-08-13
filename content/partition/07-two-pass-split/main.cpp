#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{72, 45, 88, 30, 91, 55, 60};
    auto pass = [](int s) { return s >= 60; };

    // BAD: two separate copy_if scans -- one for the predicate, one for its
    // negation -- read the whole range TWICE and duplicate the condition, so the
    // two can silently drift apart if the rule changes in only one place.
    std::vector<int> passedBad, failedBad;
    std::copy_if(v.begin(), v.end(), std::back_inserter(passedBad), pass);
    std::copy_if(v.begin(), v.end(), std::back_inserter(failedBad),
                 [](int s) { return s < 60; });   // negation repeated by hand

    // GOOD: partition_copy splits into both groups in ONE pass, stating the
    // condition once. (Or std::partition in place if you do not need a copy.)
    std::vector<int> passed, failed;
    std::partition_copy(v.begin(), v.end(),
                        std::back_inserter(passed), std::back_inserter(failed),
                        pass);

    std::cout << "passed: ";
    for (int s : passed) std::cout << s << ' ';
    std::cout << "\nfailed: ";
    for (int s : failed) std::cout << s << ' ';
    std::cout << '\n';
    return 0;
}
