#include <iostream>
#include <new>
#include <cstddef>

struct Node {
    int value;

    // Member operator new/delete (implicitly static). `new Node` and
    // `delete node` route here instead of to the global functions.
    static void* operator new(std::size_t n) {
        std::cout << "Node::operator new(" << n << ")\n";
        return ::operator new(n);
    }
    static void operator delete(void* p) noexcept {
        std::cout << "Node::operator delete\n";
        ::operator delete(p);
    }
};

int main() {
    Node* a = new Node{10};       // uses Node::operator new
    std::cout << "value = " << a->value << "\n";
    delete a;                     // uses Node::operator delete

    // A different type still uses the global operator new -- the override is
    // scoped to Node only.
    int* p = new int(5);          // no "Node::" line printed
    std::cout << "plain int = " << *p << "\n";
    delete p;
}
