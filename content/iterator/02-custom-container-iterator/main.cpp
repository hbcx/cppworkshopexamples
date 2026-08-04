// A real custom container with iterator and const_iterator.
//
// IntList is a singly linked list. By giving its iterators the standard member
// typedefs (iterator_category, value_type, ...) and the usual operators, the
// list works with range-based for AND with the standard algorithms -- std::find,
// std::distance -- none of which know anything about linked lists. The GoF point
// is here too: callers traverse the list without ever seeing a Node.

#include <iostream>
#include <iterator>   // std::forward_iterator_tag, std::distance
#include <algorithm>  // std::find
#include <cstddef>    // std::ptrdiff_t

class IntList {
    struct Node { int value; Node* next; };
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
public:
    IntList() = default;
    IntList(const IntList&) = delete;            // keep the example focused
    IntList& operator=(const IntList&) = delete;
    ~IntList() {
        for (Node* n = head_; n; ) { Node* next = n->next; delete n; n = next; }
    }

    void push_back(int v) {
        Node* n = new Node{v, nullptr};
        if (tail_) tail_->next = n; else head_ = n;
        tail_ = n;
    }

    class iterator {
        Node* node_;
    public:
        // The five typedefs the algorithm library reads via std::iterator_traits.
        using iterator_category = std::forward_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = int*;
        using reference         = int&;

        explicit iterator(Node* n) : node_(n) {}
        reference operator*() const { return node_->value; }
        iterator& operator++() { node_ = node_->next; return *this; }
        iterator  operator++(int) { iterator t = *this; node_ = node_->next; return t; }
        bool operator==(const iterator& o) const { return node_ == o.node_; }
        bool operator!=(const iterator& o) const { return node_ != o.node_; }
    };

    class const_iterator {
        const Node* node_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const int*;
        using reference         = const int&;

        explicit const_iterator(const Node* n) : node_(n) {}
        reference operator*() const { return node_->value; }
        const_iterator& operator++() { node_ = node_->next; return *this; }
        const_iterator  operator++(int) { const_iterator t = *this; node_ = node_->next; return t; }
        bool operator==(const const_iterator& o) const { return node_ == o.node_; }
        bool operator!=(const const_iterator& o) const { return node_ != o.node_; }
    };

    iterator begin() { return iterator(head_); }
    iterator end()   { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head_); }
    const_iterator end()   const { return const_iterator(nullptr); }
};

int main() {
    IntList list;
    for (int v : { 3, 1, 4, 1, 5 }) list.push_back(v);

    std::cout << "elements:";
    for (int v : list) std::cout << " " << v; // range-based for over a linked list
    std::cout << "\n";

    // Standard algorithms work unchanged -- they only speak iterator.
    auto found = std::find(list.begin(), list.end(), 4);
    std::cout << "find 4: " << (found != list.end() ? "yes" : "no") << "\n";
    std::cout << "length: " << std::distance(list.begin(), list.end()) << "\n";

    // A mutable iterator lets algorithms and loops write through it.
    for (int& v : list) v *= 10;
    std::cout << "scaled: ";
    for (int v : list) std::cout << " " << v;
    std::cout << "\n";
    return 0;
}
