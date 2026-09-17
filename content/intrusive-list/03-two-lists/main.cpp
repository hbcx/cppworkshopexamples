#include <iostream>
#include <string>
#include <utility>

// The element carries TWO hooks, so it can be threaded into two independent
// lists at the same time -- an "all tasks" list and a "ready" list -- with no
// copies. Marking a task ready adds it to the ready list without touching its
// place in the all list.
struct Task {
    std::string name;
    Task* allNext   = nullptr;   // hook 1: the "all" list
    Task* readyNext = nullptr;   // hook 2: the "ready" list
    bool  ready     = false;
    explicit Task(std::string n) : name(std::move(n)) {}
};

// Two small lists, each threading the element through its OWN hook field.
struct AllList {
    Task* head = nullptr;
    void push(Task& t) { t.allNext = head; head = &t; }
    template <class F> void each(F f) const { for (Task* p = head; p; p = p->allNext) f(*p); }
};
struct ReadyList {
    Task* head = nullptr;
    void push(Task& t) { t.readyNext = head; head = &t; t.ready = true; }
    template <class F> void each(F f) const { for (Task* p = head; p; p = p->readyNext) f(*p); }
};

int main() {
    Task a{"a"}, b{"b"}, c{"c"};
    AllList all;
    ReadyList ready;

    all.push(a);
    all.push(b);
    all.push(c);         // every task is in "all"

    ready.push(a);
    ready.push(c);        // only a and c are ready

    std::cout << "all:";
    all.each([](const Task& t) { std::cout << ' ' << t.name; });
    std::cout << '\n';                                                   // c b a

    std::cout << "ready:";
    ready.each([](const Task& t) { std::cout << ' ' << t.name; });
    std::cout << '\n';                                                   // c a

    // b is in "all" but not "ready": one object, two independent memberships.
    std::cout << "b ready? " << b.ready << '\n';                         // 0
    return 0;
}
