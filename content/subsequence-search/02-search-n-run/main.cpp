#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    // A stream of readings; we want the first RUN of three equal values in a row.
    std::vector<int> data{1, 0, 0, 0, 2, 0, 0};

    // std::search_n finds the first place where a value repeats n times in a row.
    // Here: the first run of three 0s. It returns an iterator to the start of the
    // run, or end if no such run exists.
    auto run = std::search_n(data.begin(), data.end(), 3, 0);
    if (run != data.end()) {
        std::cout << "first run of three 0s starts at index "
                  << std::distance(data.begin(), run) << '\n';
    }

    // With a predicate, "n in a row" can mean n consecutive elements satisfying a
    // condition -- here three readings in a row above a threshold, e.g. an alarm
    // that fires only after a sustained overshoot.
    std::vector<int> temps{10, 90, 20, 95, 96, 97, 30};
    auto hot = std::search_n(temps.begin(), temps.end(), 3, 50,
                             [](int reading, int limit) { return reading > limit; });
    std::cout << "three-in-a-row over 50 starts at index "
              << std::distance(temps.begin(), hot) << '\n';
    return 0;
}
