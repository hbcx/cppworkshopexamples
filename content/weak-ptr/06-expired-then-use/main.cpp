#include <iostream>
#include <memory>

struct Widget {
    int value;
    explicit Widget(int v) : value(v) {}
};

int main() {
    std::weak_ptr<Widget> w;
    {
        auto owner = std::make_shared<Widget>(7);
        w = owner;

        // --- Anti-pattern 1: check expired(), THEN use the object ---
        //   if (!w.expired()) {
        //       use(*w.lock());   // BUG: between expired() and lock(), the last
        //   }                     // owner (e.g. on another thread) can drop it,
        //                         // so expired() said "alive" but lock() is null.
        //
        // --- Anti-pattern 2: dereference lock() without checking ---
        //   w.lock()->value;      // BUG: if the object is gone, lock() is null
        //                         // and this is a null-pointer dereference (UB).

        // --- the fix: lock() ONCE, then use the result only if it is non-null ---
        if (std::shared_ptr<Widget> s = w.lock()) {
            std::cout << "alive, value = " << s->value << '\n';
        } else {
            std::cout << "already gone\n";
        }
    }   // owner goes -> Widget destroyed

    // The same idiom after expiry: one atomic check-and-acquire, then branch.
    if (std::shared_ptr<Widget> s = w.lock()) {
        std::cout << "alive, value = " << s->value << '\n';
    } else {
        std::cout << "already gone\n";
    }
    return 0;
}
