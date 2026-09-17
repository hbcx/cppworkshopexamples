#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

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
    // Remove t in O(1). We do NOT search for it: t already knows its neighbours,
    // so we splice them together. A pointer to the element is all it takes.
    void unlink(Task& t) {
        if (t.prev) t.prev->next = t.next; else head_ = t.next;
        if (t.next) t.next->prev = t.prev; else tail_ = t.prev;
        t.prev = t.next = nullptr;
    }
    template <class F>
    void for_each(F f) const { for (Task* p = head_; p; p = p->next) f(*p); }
    std::size_t size() const {
        std::size_t n = 0;
        for (Task* p = head_; p; p = p->next) ++n;
        return n;
    }
};

int main() {
    Task a{"a"}, b{"b"}, c{"c"}, d{"d"};
    TaskList list;
    for (Task* t : {&a, &b, &c, &d}) list.push_back(*t);

    std::cout << "before:";
    list.for_each([](const Task& t) { std::cout << ' ' << t.name; });
    std::cout << " (size " << list.size() << ")\n";                     // a b c d (size 4)

    // Remove c directly -- no search, O(1), given only the object.
    list.unlink(c);

    std::cout << "after unlinking c:";
    list.for_each([](const Task& t) { std::cout << ' ' << t.name; });
    std::cout << " (size " << list.size() << ")\n";                     // a b d (size 3)
    return 0;
}
