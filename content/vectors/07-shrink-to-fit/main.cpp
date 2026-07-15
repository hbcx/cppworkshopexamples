#include <iostream>
#include <vector>

int main() {
    // shrink_to_fit: ask the implementation to release spare capacity.
    {
        std::vector<int> v(1000);
        std::cout << "filled:                size=" << v.size()
                  << " capacity=" << v.capacity() << '\n';

        v.clear();   // size -> 0, but the 1000-element block is retained
        std::cout << "after clear():         size=" << v.size()
                  << " capacity=" << v.capacity() << '\n';

        v.shrink_to_fit();   // non-binding request to free the spare room
        std::cout << "after shrink_to_fit(): size=" << v.size()
                  << " capacity=" << v.capacity() << '\n';
    }

    // The portable pre-C++11 idiom: swap the buffer into a temporary empty
    // vector, which is destroyed at the end of the statement and frees the block.
    {
        std::vector<int> v(1000);
        std::vector<int>().swap(v);
        std::cout << "after swap-with-empty: size=" << v.size()
                  << " capacity=" << v.capacity() << '\n';
    }
    return 0;
}
