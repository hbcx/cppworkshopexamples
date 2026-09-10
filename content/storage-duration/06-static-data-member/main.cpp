#include <iostream>

struct Widget {
    // inline static (C++17): declared AND defined here, with no separate
    // out-of-line definition. One variable shared by the whole class.
    inline static int liveCount = 0;

    int id;
    Widget() : id(++liveCount) {}
    ~Widget() { --liveCount; }
};

int main() {
    std::cout << "live at start: " << Widget::liveCount << "\n";

    Widget a, b;
    std::cout << "after a, b:    " << Widget::liveCount
              << " (ids " << a.id << ", " << b.id << ")\n";
    {
        Widget c;
        std::cout << "inside block:  " << Widget::liveCount << " (c.id " << c.id << ")\n";
    }
    std::cout << "after block:   " << Widget::liveCount << "\n";
    return 0;
}
