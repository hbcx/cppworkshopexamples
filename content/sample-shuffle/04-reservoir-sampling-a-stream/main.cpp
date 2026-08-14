#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <vector>

int main() {
    // A stream of numbers whose length we do NOT know up front -- think of log
    // lines arriving one by one, too many to hold in memory at once.
    std::istringstream feed("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20");
    std::istream_iterator<int> first(feed), last;

    // std::sample works with INPUT iterators, so it can sample straight from the
    // stream in a single pass -- this is reservoir sampling: it keeps k slots and
    // reads each item once, never storing the whole sequence. Because the source
    // is a one-pass input iterator, the OUTPUT must be random-access, so we give
    // it a pre-sized vector (not a back_inserter).
    const int k = 5;
    std::vector<int> reservoir(k);
    std::mt19937 engine(2024);

    auto end = std::sample(first, last, reservoir.begin(), k, engine);

    std::cout << "sampled " << (end - reservoir.begin()) << " of a stream:";
    for (auto it = reservoir.begin(); it != end; ++it) std::cout << ' ' << *it;
    std::cout << '\n';

    // If the stream had fewer than k items, sample would return all of them --
    // end marks the real count, which is why we sample into [begin, end), not
    // the whole reservoir.
    return 0;
}
