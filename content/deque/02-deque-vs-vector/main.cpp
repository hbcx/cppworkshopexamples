// deque and vector trade different things:
//   deque  -- front insertion, references survive growth, but NOT contiguous
//   vector -- one contiguous block (data(), reserve, capacity), back only
// When you need contiguous storage from a deque, copy it into a vector.

#include <deque>
#include <iostream>
#include <vector>

int main() {
    std::deque<int> d = {2, 3, 4};
    d.push_front(1);          // vector has no push_front
    std::cout << "deque after push_front:";
    for (int x : d) std::cout << ' ' << x;
    std::cout << '\n';

    // A deque is not contiguous, so it has none of these:
    //   d.data();       // does not compile
    //   d.capacity();   // does not compile
    //   d.reserve(10);  // does not compile

    // Need a flat T* array (say for a C API)? Copy into a vector.
    std::vector<int> v(d.begin(), d.end());
    int* flat = v.data();     // contiguous, one block
    std::cout << "copied into a contiguous vector, first via data(): "
              << flat[0] << '\n';
    std::cout << std::boolalpha
              << "vector has capacity() >= size(): " << (v.capacity() >= v.size()) << '\n';
}
