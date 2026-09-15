#include <iostream>
#include <memory>
#include <string>

struct Base {
    virtual ~Base() { std::cout << "object destroyed\n"; }
    virtual std::string kind() const { return "Base"; }
};

struct Derived : Base {
    std::string kind() const override { return "Derived"; }
};

int main() {
    std::shared_ptr<Base> base = std::make_shared<Derived>();

    // --- the bug (NOT executed: it double-frees, which is UB) ---
    // You want a shared_ptr<Derived>, so you cast the RAW pointer and wrap it:
    //   std::shared_ptr<Derived> d(static_cast<Derived*>(base.get()));
    // Now d has its OWN control block over an object `base` already owns. Each
    // count reaches zero on its own and deletes the object -> double free. The
    // cast is not the problem; re-wrapping get() is (see the two-control-blocks
    // anti-pattern). Casting a shared_ptr must go through a *_pointer_cast,
    // which shares the control block instead of starting a new one.

    // --- the fix: static_pointer_cast shares the SAME control block ---
    std::shared_ptr<Derived> d = std::static_pointer_cast<Derived>(base);
    std::cout << "cast to " << d->kind() << '\n';
    std::cout << "one shared count, use_count = " << base.use_count() << '\n';   // 2

    // When the dynamic type is not certain, prefer dynamic_pointer_cast: it
    // returns an empty shared_ptr on a mismatch instead of a wrong pointer.
    std::shared_ptr<Derived> checked = std::dynamic_pointer_cast<Derived>(base);
    std::cout << "dynamic_pointer_cast succeeded: " << (checked ? "yes" : "no") << '\n';

    return 0;   // deleted exactly once, when the last shared owner drops
}
