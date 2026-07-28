#include <iostream>
#include <memory>

// Anti-pattern: constructing a shared_ptr from `this`. The object is already
// owned by a shared_ptr with its own control block; shared_ptr<T>(this) makes a
// SECOND control block over the same object, so two independent counts will each
// hit zero and delete it -> double free (UB).
struct Bad {
    int id;
    explicit Bad(int i) : id(i) {}
    std::shared_ptr<Bad> danglingSelf() {
        return std::shared_ptr<Bad>(this);   // BUG: brand-new second control block
    }
};

// The fix: derive from enable_shared_from_this and call shared_from_this(),
// which returns a shared_ptr that shares the ORIGINAL control block.
struct Good : std::enable_shared_from_this<Good> {
    int id;
    explicit Good(int i) : id(i) {}
    ~Good() { std::cout << "Good " << id << " destroyed once\n"; }
    std::shared_ptr<Good> self() {
        return shared_from_this();            // same control block
    }
};

int main() {
    // We do NOT call Bad::danglingSelf() and let both blocks delete -- that is
    // exactly the UB being warned about. Only the correct tool is run.
    auto g = std::make_shared<Good>(1);
    std::shared_ptr<Good> also = g->self();   // shares g's control block
    std::cout << "one object, use_count = " << g.use_count() << '\n';   // 2
    also.reset();
    std::cout << "after reset, use_count = " << g.use_count() << '\n';  // 1
    return 0;   // Good 1 destroyed exactly once
}
