#include <iostream>
#include <memory>
#include <utility>

int live = 0;   // resources still held by Model payloads

// A payload whose destructor MUST run to release its resource. In real
// code ~Payload would close a file or free memory; here it just restores
// the counter, so a skipped destructor is visible as a leak.
struct Payload {
    Payload()  { ++live; }
    ~Payload() { --live; }
};

// The type-erasure interface. The ONE line that matters is the Concept
// destructor: it is deleted through a Concept*, so it must be virtual.
struct Concept {
    virtual ~Concept() = default;   // <-- REQUIRED (see the note at the bottom)
    virtual void use() const = 0;
};
template <class T>
struct Model : Concept {
    Model(T x) : data_(std::move(x)) {}
    void use() const override {}
    T data_;
};
class Holder {
public:
    template <class T>
    Holder(T x) : self_(new Model<T>(std::move(x))) {}
    void use() const { self_->use(); }
private:
    std::unique_ptr<Concept> self_;   // deletes through Concept*
};

int main() {
    std::cout << "before:   live=" << live << "\n";
    {
        Holder h = Payload{};
        h.use();
        std::cout << "in scope: live=" << live << "\n";
    }   // ~Holder -> ~unique_ptr -> delete Concept* -> ~Model -> ~Payload
    std::cout << "after:    live=" << live << "  <- 0, the destructor ran\n";

    // THE BUG (not run here, because it is undefined behaviour):
    //   struct Concept { ~Concept() = default;      // NOT virtual
    //                    virtual void use() const = 0; };
    // Then `delete self_` deletes a Model<T> through a Concept* whose
    // destructor is not virtual. That is undefined behaviour: in practice
    // ~Model<T> is skipped, ~Payload never runs, and live stays at 1 -- a
    // leak. The fix is the single word above: virtual ~Concept().
    return 0;
}
