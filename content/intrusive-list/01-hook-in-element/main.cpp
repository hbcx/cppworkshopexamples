#include <iostream>
#include <string>
#include <utility>

// The linkage lives INSIDE the element, not in a node the list allocates. Here a
// Task carries its own next/prev pointers; the list just threads them. So the
// list itself allocates nothing -- unlike std::list, which allocates a node
// (holding a copy of the value) for every element.
struct Task {
    std::string name;
    Task* next = nullptr;
    Task* prev = nullptr;
    explicit Task(std::string n) : name(std::move(n)) {}
};

class TaskList {
    Task* head_ = nullptr;
    Task* tail_ = nullptr;
public:
    void push_back(Task& t) {
        t.prev = tail_;
        t.next = nullptr;
        if (tail_) tail_->next = &t; else head_ = &t;
        tail_ = &t;
    }
    // Iterate by following the elements' own next pointers.
    template <class F>
    void for_each(F f) const {
        for (Task* p = head_; p != nullptr; p = p->next) f(*p);
    }
    bool empty() const { return head_ == nullptr; }
};

int main() {
    // The elements live here, on the stack -- the list only threads them.
    Task a{"compile"}, b{"link"}, c{"test"};

    TaskList list;
    list.push_back(a);
    list.push_back(b);
    list.push_back(c);

    std::cout << "tasks in order:";
    list.for_each([](const Task& t) { std::cout << ' ' << t.name; });
    std::cout << '\n';

    // No allocation happened for the list: the Task objects already existed, and
    // the links are members inside them.
    std::cout << "list empty? " << list.empty() << '\n';   // 0
    return 0;
}
