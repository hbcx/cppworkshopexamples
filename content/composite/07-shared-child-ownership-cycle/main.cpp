#include <iostream>
#include <memory>

// Track live nodes so a leak is observable in the output.
struct Counted {
    Counted() { ++live(); }
    virtual ~Counted() { --live(); }
    static int& live() { static int n = 0; return n; }
};

// ANTI-PATTERN: parent and child point at each other with shared_ptr, forming an
// ownership CYCLE. Each keeps the other's reference count above zero, so when the
// external handles go out of scope neither node is ever freed -- a classic leak
// that a raw unique_ptr tree cannot have, but a strong back-pointer creates.
struct BadNode : Counted {
    std::shared_ptr<BadNode> child;
    std::shared_ptr<BadNode> parent;    // strong back-pointer -- BUG
};

// FIX: ownership goes ONE way -- the parent owns the child. The back-pointer is a
// weak_ptr, which does not raise the reference count, so there is no cycle and
// both nodes are freed when the external handles go away.
struct GoodNode : Counted {
    std::shared_ptr<GoodNode> child;
    std::weak_ptr<GoodNode> parent;     // non-owning back-pointer
};

int main() {
    std::cout << "live at start: " << Counted::live() << "\n";      // 0

    {
        auto p = std::make_shared<GoodNode>();
        auto c = std::make_shared<GoodNode>();
        p->child = c;
        c->parent = p;                  // weak back-pointer -- no cycle
    }
    std::cout << "live after weak-parent scope: " << Counted::live() << "\n";   // 0 -- freed

    {
        auto p = std::make_shared<BadNode>();
        auto c = std::make_shared<BadNode>();
        p->child = c;
        c->parent = p;                  // cycle formed
    }   // p and c leave scope, but the cycle keeps both alive
    std::cout << "live after cyclic scope: " << Counted::live() << "\n";        // 2 -- leaked
    return 0;
}
