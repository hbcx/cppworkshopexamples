#include <iostream>
#include <memory>

// A pimpl-style type. Impl is forward-declared here; the special members that
// destroy the unique_ptr<Impl> must be defined only AFTER Impl becomes complete.
class Gadget {
public:
    Gadget();
    ~Gadget();               // declared here, DEFINED below where Impl is complete
    void use();
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// ANTI-PATTERN (described, not compiled): if instead you let the destructor be
// implicit -- omit the declaration, or write  ~Gadget() = default;  HERE while
// Impl is still only forward-declared -- the compiler must instantiate
// unique_ptr<Impl>'s deleter against an INCOMPLETE Impl. Its deleter does a
// static_assert(sizeof(Impl) > 0), so this is a hard COMPILE error ("deleting
// incomplete type"), fired wherever a Gadget is destroyed. It cannot be shown
// running because the program does not build. The same applies to a defaulted
// move constructor and move assignment.

// Impl is completed here.
struct Gadget::Impl {
    int uses = 0;
};

// FIX: define the special members now, where Impl is a complete type.
Gadget::Gadget() : impl_(new Impl) {}
Gadget::~Gadget() = default;         // OK here: Impl is complete

void Gadget::use() {
    ++impl_->uses;
    std::cout << "used " << impl_->uses << " time(s)\n";
}

int main() {
    Gadget g;
    g.use();
    g.use();
    return 0;
}
