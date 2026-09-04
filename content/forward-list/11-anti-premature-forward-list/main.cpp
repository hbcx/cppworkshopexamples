#include <forward_list>
#include <list>
#include <iostream>
#include <cstddef>

// ANTI-PATTERN: the code needed size() and back(), so a forward_list gets
// wrapped with a hand-maintained counter and tail iterator -- reinventing what
// std::list already does, with more state to keep correct and MORE memory used.
class Queue {
    std::forward_list<int> data_;
    std::forward_list<int>::iterator tail_;   // extra per-CONTAINER state...
    std::size_t size_ = 0;                     // ...that must be updated on every op
public:
    Queue() : tail_(data_.before_begin()) {}
    void push(int v) { tail_ = data_.insert_after(tail_, v); ++size_; }
    std::size_t size() const { return size_; }
    int back() const { return *tail_; }        // hope tail_ is still valid...
};

int main() {
    // The wrapper works, but look at what it took: a tail iterator and a size
    // counter kept in sync by hand, exactly the bookkeeping the "lighter"
    // container was chosen to avoid.
    Queue q;
    q.push(10);
    q.push(20);
    q.push(30);
    std::cout << "hand-rolled queue: size=" << q.size() << " back=" << q.back() << "\n";

    // FIX: if you need size and back, std::list already maintains both -- O(1)
    // size() since C++11 and O(1) back() -- with no extra fields of your own.
    std::list<int> better{10, 20, 30};
    std::cout << "std::list:         size=" << better.size()
              << " back=" << better.back() << "\n";

    std::cout << "The one saved pointer per node was never the real constraint "
                 "once size/back were needed.\n";
    return 0;
}
