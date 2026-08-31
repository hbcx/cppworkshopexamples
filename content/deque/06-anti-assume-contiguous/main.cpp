// Anti-pattern: treating a deque as one contiguous array.
//
// A deque is separate chunks, not a single block, so &d[0] walked as a flat
// buffer runs off the end of the first chunk into unrelated memory. deque has no
// data() for exactly this reason. Copy into a vector when you need a T* array.

#include <deque>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::deque<int> d = {1, 2, 3, 4, 5};

    // BAD (undefined for a large deque -- described, not run):
    //   const int* p = &d[0];
    //   int total = 0;
    //   for (std::size_t i = 0; i < d.size(); ++i) total += p[i];
    //   // p[i] assumes one contiguous block; past a chunk boundary this reads
    //   // unrelated memory. There is no d.data() to hand out either.

    // GOOD 1: iterate the deque directly (iterators cross chunks correctly).
    int viaIterators = std::accumulate(d.begin(), d.end(), 0);
    std::cout << "sum via deque iterators: " << viaIterators << '\n';

    // GOOD 2: need a real T* array? Copy into a contiguous vector.
    std::vector<int> flat(d.begin(), d.end());
    const int* p = flat.data();
    int viaData = 0;
    for (std::size_t i = 0; i < flat.size(); ++i) viaData += p[i];
    std::cout << "sum via vector data(): " << viaData << '\n';
}
