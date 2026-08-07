#include <iostream>

// A value that owns a numbered handle. Assignment must release the old handle
// and then copy the other's. The trap is what happens when the other object
// IS this object.
class Handle {
public:
    explicit Handle(int id) : id_(id) {}

    // BAD: no self-assignment guard.
    Handle& assignBad(const Handle& o) {
        release();          // give up our handle...
        id_ = o.id_;        // ...but if &o == this, o.id_ is now -1
        return *this;
    }

    // GOOD: guard against self-assignment (copy-and-swap would also fix it).
    Handle& assignGood(const Handle& o) {
        if (this != &o) {
            release();
            id_ = o.id_;
        }
        return *this;
    }

    int id() const { return id_; }

private:
    void release() { id_ = -1; }   // mark the handle as released
    int id_;
};

int main() {
    Handle a(7);
    a.assignBad(a);          // self-assignment through the buggy path
    std::cout << "bad  self-assign: id=" << a.id() << "  <- -1, corrupted\n";

    Handle b(7);
    b.assignGood(b);         // self-assignment through the guarded path
    std::cout << "good self-assign: id=" << b.id() << "  <- 7, intact\n";
    return 0;
}
