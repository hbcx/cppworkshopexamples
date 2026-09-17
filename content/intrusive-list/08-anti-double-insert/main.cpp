#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

struct Node {
    std::string name;
    Node* next   = nullptr;
    bool  linked = false;
    explicit Node(std::string n) : name(std::move(n)) {}
};

class List {
    Node* head_ = nullptr;
public:
    // The buggy push: it does not check whether n is already linked somewhere.
    void push_front_unchecked(Node& n) { n.next = head_; head_ = &n; n.linked = true; }
    // The safe push: refuse a node that is already linked (move = unlink first).
    bool push_front_safe(Node& n) {
        if (n.linked) return false;
        n.next = head_; head_ = &n; n.linked = true;
        return true;
    }
    std::size_t size() const {
        std::size_t k = 0;
        for (Node* p = head_; p; p = p->next) ++k;
        return k;
    }
};

int main() {
    Node a{"a"}, b{"b"}, c{"c"};
    List first, second;

    first.push_front_unchecked(a);       // first:  a
    second.push_front_unchecked(c);
    second.push_front_unchecked(b);      // second: b -> c

    // --- the trap ---
    // a is already linked in `first`. Inserting it into `second` through the SAME
    // next hook overwrites a.next to point at second's head (b). Now walking
    // `first` from a runs straight into second's nodes: first is corrupted.
    second.push_front_unchecked(a);      // second: a -> b -> c ; first is now broken

    std::cout << "second size: " << second.size() << '\n';                 // 3
    std::cout << "first size (should be 1): " << first.size() << '\n';      // 3  <-- corrupted

    // --- the fix: a guard that refuses an already-linked node ---
    Node d{"d"};
    List third;
    third.push_front_safe(d);                       // ok, d was free
    bool again = third.push_front_safe(d);          // d already linked -> refused
    std::cout << "re-inserting an already-linked node accepted? " << again << '\n';   // 0
    return 0;
}
