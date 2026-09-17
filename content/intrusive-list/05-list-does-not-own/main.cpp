#include <cstddef>
#include <deque>
#include <iostream>
#include <string>
#include <utility>

struct Node {
    std::string name;
    Node* next = nullptr;
    Node* prev = nullptr;
    ~Node() { ++destroyed; }
    explicit Node(std::string n) : name(std::move(n)) {}
    static int destroyed;
};
int Node::destroyed = 0;

class List {
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
public:
    void push_back(Node& n) {
        n.prev = tail_; n.next = nullptr;
        if (tail_) tail_->next = &n; else head_ = &n;
        tail_ = &n;
    }
    // Detach every link. This does NOT destroy the nodes -- the list does not
    // own them.
    void clear() {
        for (Node* p = head_; p != nullptr; ) {
            Node* nx = p->next;
            p->next = p->prev = nullptr;
            p = nx;
        }
        head_ = tail_ = nullptr;
    }
    std::size_t size() const {
        std::size_t n = 0;
        for (Node* p = head_; p; p = p->next) ++n;
        return n;
    }
};

int main() {
    // The elements are OWNED here, in a deque with stable addresses. The list
    // only threads them -- it is not their owner.
    std::deque<Node> pool;
    pool.emplace_back("a");
    pool.emplace_back("b");
    pool.emplace_back("c");

    List list;
    for (Node& n : pool) list.push_back(n);
    std::cout << "list size " << list.size()
              << ", destroyed so far " << Node::destroyed << '\n';       // 3, 0

    list.clear();   // the list lets go of the nodes -- it does NOT destroy them
    std::cout << "after clear: list size " << list.size()
              << ", destroyed " << Node::destroyed << '\n';              // 0, 0
    std::cout << "elements still alive: "
              << pool[0].name << ' ' << pool[1].name << ' ' << pool[2].name << '\n';

    // The nodes die only when their real owner -- the pool -- goes away, which
    // happens as main returns.
    return 0;
}
