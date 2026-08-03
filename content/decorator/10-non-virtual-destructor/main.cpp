#include <iostream>
#include <memory>
#include <utility>

// A component whose decorators hold resources released in their destructor.
// The base destructor is VIRTUAL here -- that is the fix. See main() for what
// goes wrong without it.
struct Handle {
    virtual void use() const = 0;
    virtual ~Handle() = default;      // FIX: virtual, so deleting through Handle*
};                                    // runs the most-derived destructor first.

class RealHandle : public Handle {
public:
    void use() const override { std::cout << "use\n"; }
    ~RealHandle() override { std::cout << "release: real handle\n"; }
};

class TracingHandle : public Handle {
public:
    explicit TracingHandle(std::unique_ptr<Handle> inner) : inner_(std::move(inner)) {}
    void use() const override { std::cout << "trace: "; inner_->use(); }
    ~TracingHandle() override { std::cout << "release: tracing layer\n"; }
private:
    std::unique_ptr<Handle> inner_;
};

int main() {
    // With a VIRTUAL base destructor, deleting the chain through the base
    // interface runs every layer's destructor, outermost first, so both
    // resources are released.
    std::unique_ptr<Handle> h =
        std::make_unique<TracingHandle>(std::make_unique<RealHandle>());
    h->use();
    // h goes out of scope here: "release: tracing layer" then "release: real handle".

    // Were Handle's destructor NON-virtual, deleting through a Handle* would run
    // only ~Handle and skip ~TracingHandle: the tracing layer's cleanup would not
    // run and its owned inner handle would never be freed -- undefined behaviour,
    // so it is described rather than executed here.
    return 0;
}
