#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int main() {
    std::list<int> nodes{10, 20, 30};

    // Remember a position and take a raw pointer at the element behind it.
    auto middle = ++nodes.begin();      // -> 20
    const int* address = &*middle;

    // Hammer the list: grow it at both ends, insert in the middle, erase a
    // neighbour. A vector would have reallocated several times over by now.
    for (int i = 0; i < 100; ++i) {
        nodes.push_back(i);
        nodes.push_front(-i);
    }
    nodes.insert(nodes.begin(), 999);
    nodes.erase(--nodes.end());         // erase the last element -- not ours

    // Everything we saved still refers to the same node, at the same address.
    std::cout << "iterator still reads: " << *middle << '\n';
    std::cout << "pointer still reads:  " << *address << '\n';
    std::cout << "node never moved:     "
              << (&*middle == address ? "yes" : "no") << '\n';

    // Erasing a NEIGHBOUR leaves our iterator alone -- only iterators to the
    // erased element itself are invalidated.
    nodes.erase(std::prev(middle));
    std::cout << "after erasing its neighbour: " << *middle
              << " (still node " << (&*middle == address ? "20" : "?") << ")\n";

    // The contrast: a vector's guarantee is the opposite one. Growth moves every
    // element, so any pointer taken beforehand is dangling afterwards.
    std::vector<int> v{10, 20, 30};
    const int* before = v.data();
    v.reserve(v.capacity() + 1);        // force exactly one reallocation
    std::cout << "vector storage moved: "
              << (v.data() != before ? "yes -- old pointers now dangle" : "no") << '\n';
    return 0;
}
