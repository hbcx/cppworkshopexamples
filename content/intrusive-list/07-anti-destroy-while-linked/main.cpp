#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

struct Node;

class List {
    Node* head_ = nullptr;
public:
    void push_front(Node& n);
    void unlink(Node& n);
    Node* head() const { return head_; }
    std::size_t size() const;
};

// An auto-unlink hook: the element remembers which list it is in and removes
// itself in its OWN destructor, so it can never die while still linked.
struct Node {
    std::string name;
    Node* next   = nullptr;
    List* owner  = nullptr;
    explicit Node(std::string n) : name(std::move(n)) {}
    ~Node() { if (owner) owner->unlink(*this); }   // auto-unlink on destruction
};

void List::push_front(Node& n) { n.next = head_; n.owner = this; head_ = &n; }
void List::unlink(Node& n) {
    Node** pp = &head_;
    while (*pp && *pp != &n) pp = &(*pp)->next;
    if (*pp) { *pp = n.next; n.next = nullptr; n.owner = nullptr; }
}
std::size_t List::size() const {
    std::size_t k = 0;
    for (Node* p = head_; p; p = p->next) ++k;
    return k;
}

int main() {
    List list;
    Node a{"a"};
    list.push_front(a);

    {
        Node temp{"temp"};
        list.push_front(temp);
        std::cout << "inside scope, list size = " << list.size() << '\n';   // 2
    }   // temp is destroyed HERE -- its destructor unlinks it from the list

    // --- the trap (WITHOUT the auto-unlink hook) ---
    // If Node did not unlink itself on destruction, the list would still hold a
    // pointer to `temp`, which no longer exists; iterating would dereference
    // freed memory -- undefined behaviour. We never do that.
    //
    // --- the fix: the destructor unlinks, so the list stays consistent ---
    std::cout << "after temp's scope, list size = " << list.size() << '\n';  // 1
    std::cout << "remaining:";
    for (Node* p = list.head(); p; p = p->next) std::cout << ' ' << p->name;
    std::cout << '\n';                                                        // a
    return 0;
}
