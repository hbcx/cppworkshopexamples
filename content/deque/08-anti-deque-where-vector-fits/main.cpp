// Anti-pattern: choosing a deque that only ever push_backs and indexes.
//
// With no front operations and no need for reference stability, a deque is just
// a vector with worse cache behaviour and no data(). Switch the type to vector:
// same code, contiguous storage, faster iteration.

#include <deque>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    // BAD shape: a deque used only from the back and by index.
    std::deque<int> d;
    for (int i = 0; i < 5; ++i) d.push_back(i * i);
    long dequeSum = std::accumulate(d.begin(), d.end(), 0L);

    // GOOD: the same operations on a vector -- one contiguous block, has data().
    std::vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i * i);
    long vectorSum = std::accumulate(v.begin(), v.end(), 0L);

    std::cout << "deque sum:  " << dequeSum << '\n';
    std::cout << "vector sum: " << vectorSum << '\n';
    std::cout << std::boolalpha
              << "same result, vector is the better fit here: "
              << (dequeSum == vectorSum) << '\n';
}
