// std::deque grows cheaply at BOTH ends -- push_back and push_front are each
// O(1) -- and still supports random access. A vector only grows cheaply at the
// back.

#include <deque>
#include <iostream>

int main() {
    std::deque<int> d;

    d.push_back(2);       // back:  [2]
    d.push_back(3);       // back:  [2, 3]
    d.push_front(1);      // front: [1, 2, 3]
    d.push_front(0);      // front: [0, 1, 2, 3]

    std::cout << "contents:";
    for (int x : d) std::cout << ' ' << x;
    std::cout << '\n';

    std::cout << "d[2]      = " << d[2] << '\n';     // random access
    std::cout << "d.front() = " << d.front() << '\n';
    std::cout << "d.back()  = " << d.back() << '\n';

    d.pop_front();        // drop 0
    d.pop_back();         // drop 3
    std::cout << "after popping both ends:";
    for (int x : d) std::cout << ' ' << x;
    std::cout << '\n';
}
