#include <iostream>
#include <iterator>
#include <list>
#include <vector>

// O(1) for random-access iterators: jump directly.
template <class It>
void myAdvance(It& it, int n, std::random_access_iterator_tag) {
    it += n;
    std::cout << "random-access: O(1) jump\n";
}
// O(n) for weaker iterators: step one at a time. A bidirectional tag binds
// here through tag inheritance (bidirectional derives from forward).
template <class It>
void myAdvance(It& it, int n, std::forward_iterator_tag) {
    for (int i = 0; i < n; ++i) ++it;
    std::cout << "forward/bidirectional: O(n) steps\n";
}
// Dispatcher: read the iterator's category and pass it as a tag.
template <class It>
void myAdvance(It& it, int n) {
    myAdvance(it, n, typename std::iterator_traits<It>::iterator_category{});
}

int main() {
    std::vector<int> v = {0, 1, 2, 3, 4, 5};
    std::vector<int>::iterator vit = v.begin();
    myAdvance(vit, 3);            // vector iterator: random access -> O(1)
    std::cout << "vector[3] = " << *vit << "\n";

    std::list<int> l = {0, 1, 2, 3, 4, 5};
    std::list<int>::iterator lit = l.begin();
    myAdvance(lit, 3);           // list iterator: bidirectional -> forward overload
    std::cout << "list[3] = " << *lit << "\n";
    return 0;
}
