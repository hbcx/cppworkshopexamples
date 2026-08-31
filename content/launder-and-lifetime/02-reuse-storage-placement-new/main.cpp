// Reusing storage: destroy the old object, then placement-new a new one into the
// same bytes. Use the pointer the new-expression returns -- it always refers to
// the object just built.

#include <iostream>
#include <new>

struct Widget {
    int id;
    explicit Widget(int i) : id(i) {}
};

int main() {
    alignas(Widget) unsigned char storage[sizeof(Widget)];

    Widget* a = new (storage) Widget(1);
    std::cout << "first object:  id = " << a->id << '\n';
    a->~Widget();                              // end the first object's lifetime

    Widget* b = new (storage) Widget(2);       // reuse the same bytes
    std::cout << "second object: id = " << b->id << '\n';   // use the returned ptr
    b->~Widget();

    std::cout << "same storage, two objects over time\n";
}
