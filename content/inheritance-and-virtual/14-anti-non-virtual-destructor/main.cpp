#include <iostream>

// Anti-pattern: a base used polymorphically with a NON-virtual destructor. We
// track a "live resource" count to see whether the derived cleanup runs. Because
// deleting through the bad base is undefined behavior, we do NOT run that delete;
// we run the fix and describe the trap.

int liveResources = 0;   // a stand-in for memory / files / locks a derived owns

// FIXED base: virtual destructor.
struct GoodBase {
    virtual ~GoodBase() = default;
};
struct GoodDerived : GoodBase {
    GoodDerived()          { ++liveResources; }
    ~GoodDerived() override { --liveResources; }   // releases the resource
};

// BROKEN base: non-virtual destructor. The classes themselves are fine; the bug
// is only triggered by deleting a GoodDerived-shaped object through BadBase*.
struct BadBase {
    ~BadBase() = default;                          // <-- should be virtual
};
struct BadDerived : BadBase {
    BadDerived()  { ++liveResources; }
    ~BadDerived() { --liveResources; }
};

int main() {
    // FIXED: delete through the base pointer runs ~GoodDerived, releasing it.
    GoodBase* p = new GoodDerived();
    delete p;
    std::cout << "good: live resources after delete = " << liveResources << "\n";  // 0

    // BROKEN, described but NOT executed:
    //   BadBase* q = new BadDerived();
    //   delete q;   // undefined behavior: runs only ~BadBase, ~BadDerived skipped,
    //               // so liveResources would stay 1 -- the resource leaks.

    // BadDerived is correct when destroyed by its real type (no base-pointer delete):
    { BadDerived d; (void)d; }   // ~BadDerived runs at scope end
    std::cout << "bad destroyed by real type: live resources = " << liveResources << "\n"; // 0
    return 0;
}
