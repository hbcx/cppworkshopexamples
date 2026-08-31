#include <iostream>
#include <new>       // ::operator new / delete, placement new
#include <string>
#include <utility>

struct Widget {
    std::string name;
    int id;
    Widget(std::string n, int i) : name(std::move(n)), id(i) {
        std::cout << "Widget(" << name << ", " << id << ") constructed\n";
    }
    ~Widget() { std::cout << "~Widget(" << name << ")\n"; }
};

int main() {
    // A `new` expression does two separate things:
    //   1) call operator new to obtain raw memory (no object exists yet)
    //   2) construct the object in that memory
    // Here we do the two steps by hand to make the split visible.
    void* raw = ::operator new(sizeof(Widget));   // step 1: allocate only
    std::cout << "raw memory allocated\n";
    Widget* w = new (raw) Widget("manual", 1);    // step 2: placement-construct

    std::cout << "using " << w->name << " #" << w->id << "\n";

    // `delete` also does two things, in the opposite order:
    //   1) run the destructor
    //   2) call operator delete to release the memory
    w->~Widget();                                 // step 1: destroy
    ::operator delete(raw);                       // step 2: deallocate
    std::cout << "raw memory released\n";

    // The ordinary form fuses both steps -- this is what you write day to day.
    Widget* w2 = new Widget("normal", 2);
    delete w2;
}
