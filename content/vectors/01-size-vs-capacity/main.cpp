#include <iostream>
#include <vector>

// size()     = how many elements are currently in the vector.
// capacity() = how many it can hold before it must allocate a bigger block.
// They are different numbers: size grows one at a time, capacity in jumps.
int main() {
    std::vector<int> v;
    std::cout << "start:                 size=" << v.size()
              << " capacity=" << v.capacity() << '\n';

    for (int i = 1; i <= 8; ++i) {
        v.push_back(i);
        std::cout << "after push_back(" << i << "):    size=" << v.size()
                  << " capacity=" << v.capacity() << '\n';
    }

    std::cout << "capacity >= size always? "
              << (v.capacity() >= v.size() ? "yes" : "no") << '\n';
    return 0;
}
