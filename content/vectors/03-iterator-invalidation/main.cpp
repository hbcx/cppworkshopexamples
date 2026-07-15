#include <iostream>
#include <vector>

// When a push_back exceeds capacity(), the vector allocates a bigger block,
// moves the elements over, and frees the old one. Anything that pointed into the
// old block -- pointers, references, iterators -- is now dangling.
int main() {
    std::vector<int> v;
    v.reserve(2);            // capacity is now exactly 2
    v.push_back(10);
    v.push_back(20);         // size == capacity == 2: the next push_back must grow

    const int* saved = v.data();   // a pointer into the current storage
    std::cout << "before: size=" << v.size()
              << " capacity=" << v.capacity() << '\n';

    v.push_back(30);         // exceeds capacity -> reallocation

    std::cout << "after:  size=" << v.size()
              << " capacity=" << v.capacity() << '\n';

    // We compare the pointers (a defined operation); we do NOT read *saved,
    // which would now be undefined behaviour.
    const bool moved = v.data() != saved;
    std::cout << "storage relocated? " << (moved ? "yes" : "no") << '\n';
    std::cout << "=> the saved pointer, and any reference or iterator taken "
                 "before the growth, now dangles.\n";
    return 0;
}
