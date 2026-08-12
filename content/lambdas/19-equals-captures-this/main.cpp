#include <iostream>
#include <functional>

struct Widget {
    int value = 10;

    // This LOOKS like it captures value by value, but [=] in a member function
    // captures THIS (a pointer). The lambda reads value through the object, so it
    // dangles if the Widget is destroyed -- despite the [=].
    std::function<int()> getReader() {
        return [=] { return value; };   // captures this, NOT a copy of value
    }
    // FIX: capture a copy of the whole object (C++17), so the closure is self-contained.
    std::function<int()> getSafeReader() {
        return [*this] { return value; };
    }
};

int main() {
    std::function<int()> reader, safe;
    {
        Widget w;
        w.value = 99;
        reader = w.getReader();     // holds this -> &w
        safe   = w.getSafeReader(); // holds a copy of w
    }   // w is destroyed here

    // Calling reader() now would read a destroyed Widget through a dangling this:
    // undefined behavior, so we do NOT. safe has its own copy and is fine.
    std::cout << "safe reader ([*this] copy): " << safe() << '\n';   // 99
    // std::cout << reader();   // <- UB: reads a destroyed Widget

    // The point: [=] does NOT copy the members; it copies the pointer. C++20
    // deprecated the implicit this capture in [=] for exactly this confusion --
    // write [*this] to copy the object, or name the members you actually want.
    return 0;
}
