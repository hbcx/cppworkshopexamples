#include <iostream>
#include <memory>

struct Widget {
    int id;
    explicit Widget(int i) : id(i) { std::cout << "Widget " << id << " built\n"; }
    ~Widget() { std::cout << "Widget " << id << " destroyed\n"; }
};

int main() {
    std::weak_ptr<Widget> w;
    {
        auto s = std::make_shared<Widget>(1);
        w = s;
        std::cout << "alive: use_count = " << w.use_count()
                  << ", expired = " << (w.expired() ? "yes" : "no") << '\n';   // 1, no
    }   // s goes: the Widget is destroyed NOW...

    // ...but the weak_ptr is still valid to query. That works because the control
    // block (which holds the counts) outlives the object: a weak_ptr keeps the
    // control block alive so lock()/expired() stay safe to call after the object
    // is gone.
    std::cout << "after owner gone: use_count = " << w.use_count()
              << ", expired = " << (w.expired() ? "yes" : "no") << '\n';       // 0, yes
    std::cout << "lock() returns " << (w.lock() ? "a pointer" : "null") << '\n';  // null

    w.reset();   // drop the last weak reference -> the control block is freed too
    std::cout << "done\n";
    return 0;
}
