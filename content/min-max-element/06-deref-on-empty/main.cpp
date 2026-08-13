#include <algorithm>
#include <vector>
#include <iostream>

// Imagine this comes from a sensor read or a query that can legitimately be empty.
std::vector<int> loadReadings() {
    return {};
}

int main() {
    std::vector<int> readings = loadReadings();

    // BAD: max_element on a possibly-empty range returns end(), and dereferencing
    // end() is undefined behavior -- a read one past the last element. Shown only
    // as a comment, because running it is UB (it may crash or print garbage):
    //     int worst = *std::max_element(readings.begin(), readings.end());

    // GOOD: min/max_element return end() for an empty range exactly so you can
    // check first. Compare against end(), then dereference only when there is one.
    auto it = std::max_element(readings.begin(), readings.end());
    if (it != readings.end())
        std::cout << "max reading: " << *it << '\n';
    else
        std::cout << "no readings to report\n";
    return 0;
}
