// An object's lifetime is the window between its constructor finishing and its
// destructor starting. The storage can exist outside that window, but there is
// no object to use. Placement new and a manual destructor make the window
// explicit.

#include <iostream>
#include <new>

struct Widget {
    int id;
    static bool alive;
    explicit Widget(int i) : id(i) { alive = true; }
    ~Widget() { alive = false; }
};
bool Widget::alive = false;

int main() {
    alignas(Widget) unsigned char storage[sizeof(Widget)];

    std::cout << std::boolalpha;
    std::cout << "storage exists, object alive? " << Widget::alive << '\n';

    Widget* w = new (storage) Widget(7);   // lifetime BEGINS
    std::cout << "after construction, alive? " << Widget::alive
              << " (id = " << w->id << ")\n";

    w->~Widget();                          // lifetime ENDS
    std::cout << "after destruction, alive? " << Widget::alive << '\n';
    // The bytes are still here, but there is no Widget to touch anymore.
}
