// The Non-Virtual Interface (NVI) idiom.
//
// The public method a client calls is non-virtual and lives entirely in the
// base; the point that varies is a PRIVATE virtual the base calls in the middle.
// This lets the base wrap every call in fixed work no subclass can skip or
// reorder -- here logging and timing around the real work -- while the subclass
// customises only the core. The customisation point being private is the key:
// a subclass overrides it, but only the base ever calls it, and always with its
// pre/post steps in place. This is Herb Sutter's advice: prefer to make public
// functions non-virtual and virtual functions private (or protected).

#include <iostream>

class Request {
public:
    virtual ~Request() = default;

    // Public, non-virtual: the client calls this, and the base controls the
    // whole shape -- validate, then the variable core, then finish -- so the
    // cross-cutting steps always run and always in this order.
    void handle() {
        std::cout << "[log] begin\n";
        if (!validate()) {              // fixed pre-step
            std::cout << "[log] rejected\n";
            return;
        }
        process();                      // the ONE varying step
        std::cout << "[log] end\n";     // fixed post-step
    }

private:
    // The customisation point: private virtual. Subclasses override it; only
    // handle() ever calls it, so it can never be invoked without the wrapper.
    virtual void process() = 0;

    // A fixed step the base owns; not an extension point.
    bool validate() { return true; }
};

class GetRequest : public Request {
    void process() override { std::cout << "read the resource\n"; }
};

class PostRequest : public Request {
    void process() override { std::cout << "write the resource\n"; }
};

int main() {
    GetRequest{}.handle();
    PostRequest{}.handle();
    return 0;
}
