// Per-type static state with CRTP.
//
// Because Base<Derived> is a distinct type per derived class, a static member in
// the base exists once PER derived type. ObjectCounter<T> uses that to give each
// type its own live-instance count from one reusable base -- Widgets and Gadgets
// are counted separately, though the counting is written once.

#include <iostream>

template <class Derived>
class ObjectCounter {
public:
    ObjectCounter() { ++count_; }
    ObjectCounter(const ObjectCounter&) { ++count_; }
    ~ObjectCounter() { --count_; }
    static int alive() { return count_; }
protected:
    static int count_; // one instance PER Derived type, not shared
};

// Each instantiation gets its own definition -> its own counter.
template <class Derived>
int ObjectCounter<Derived>::count_ = 0;

struct Widget : ObjectCounter<Widget> {};
struct Gadget : ObjectCounter<Gadget> {};

int main() {
    Widget w1;
    Widget w2;
    Gadget g1;

    std::cout << "widgets alive = " << Widget::alive() << "\n"; // 2
    std::cout << "gadgets alive = " << Gadget::alive() << "\n"; // 1

    {
        Widget w3;
        std::cout << "widgets in inner scope = " << Widget::alive() << "\n"; // 3
    }

    std::cout << "widgets after scope = " << Widget::alive() << "\n"; // 2
    std::cout << "gadgets unaffected  = " << Gadget::alive() << "\n"; // 1
}
