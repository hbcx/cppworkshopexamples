// Anti-pattern: a polymorphic base with a non-virtual destructor.
//
// A Template Method base is meant to be inherited and is often held and deleted
// through a base pointer. If its destructor is not virtual, deleting a derived
// object through a base pointer is undefined behaviour: only ~Base runs, the
// derived destructor and its members' destructors are skipped, so whatever the
// derived class owns leaks. The bad case is described in the comment and NOT
// compiled (both because it is UB and because deleting through a base pointer
// with a non-virtual destructor is a hard error under -Werror). The good version
// gives the base a virtual destructor and runs, freeing everything in order.
//
// ---- BAD (undefined behaviour -- described, not compiled) ----
//
//   struct BaseBad {
//       virtual void step() { std::cout << "base\n"; }
//       ~BaseBad() {}                 // NON-virtual, yet the class is polymorphic
//   };
//   struct DerivedBad : BaseBad {
//       Resource res_;                // owns something that must be freed
//       void step() override { std::cout << "derived\n"; }
//   };
//   BaseBad* p = new DerivedBad();
//   delete p;   // UB: ~DerivedBad and ~Resource never run -> res_ leaks
//
// Rule: a base with any virtual function that is deleted polymorphically needs a
// public virtual destructor (or a protected non-virtual one to forbid it).

#include <iostream>
#include <memory>

struct Resource {
    Resource()  { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource freed\n"; }
};

// ---- GOOD: virtual destructor, so deleting through Base* runs ~Derived ----
struct BaseGood {
    void run() { step(); }
    virtual void step() { std::cout << "base step\n"; }
    virtual ~BaseGood() { std::cout << "~BaseGood\n"; } // enables correct polymorphic delete
};

struct DerivedGood : BaseGood {
    Resource res_;
    void step() override { std::cout << "derived step\n"; }
    ~DerivedGood() override { std::cout << "~DerivedGood\n"; }
};

int main() {
    std::unique_ptr<BaseGood> p(new DerivedGood()); // held by base pointer
    p->run();
    // Destroying p through BaseGood* runs ~DerivedGood, then ~Resource, then
    // ~BaseGood -- everything the derived object owns is freed, in order.
    return 0;
}
