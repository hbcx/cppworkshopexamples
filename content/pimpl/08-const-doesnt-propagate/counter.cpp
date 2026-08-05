#include "counter.hpp"

struct Counter::Impl {
    int reads = 0;
    int value = 7;
};

Counter::Counter() : impl_(std::make_unique<Impl>()) {}
Counter::~Counter() = default;

int Counter::reads() const { return impl_->reads; }

// operator-> on a CONST unique_ptr still yields a non-const Impl*, so this const
// method can write to the Impl. The compiler does not complain.
int Counter::peekBad() const {
    return ++impl_->reads; // <-- a const method just mutated the object
}

// The honest version: it reads and does not mutate. To make the compiler ENFORCE
// this, route access through a const accessor:
//     const Impl& d() const { return *impl_; }   // const method sees const Impl
//     Impl&       d()       { return *impl_; }
// Then peekBad's ++d().reads would fail to compile.
int Counter::lookGood() const {
    return impl_->value;
}
