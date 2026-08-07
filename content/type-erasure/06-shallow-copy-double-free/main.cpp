#include <iostream>
#include <memory>
#include <string>
#include <utility>

int releaseCount = 0;   // total releases; a double free shows up as a repeat
int nextId = 0;         // a distinct id per Model, to see WHICH one is released

struct Concept {
    virtual ~Concept() = default;
    virtual void release() = 0;
    virtual std::unique_ptr<Concept> clone() const = 0;
};
template <class T>
struct Model : Concept {
    Model(T x) : data_(std::move(x)), id_(nextId++) {}
    Model(const Model& o) : data_(o.data_), id_(nextId++) {}  // a clone gets a fresh id
    void release() override { std::cout << "  release id=" << id_ << "\n"; ++releaseCount; }
    std::unique_ptr<Concept> clone() const override {
        return std::unique_ptr<Concept>(new Model<T>(*this));
    }
    T data_;
    int id_;
};

// BAD: made copyable by copying the raw pointer. Two wrappers then share
// one Model and both release it -- the same object freed twice.
class Bad {
public:
    template <class U>
    Bad(U x) : self_(new Model<U>(std::move(x))) {}
    Bad(const Bad& o) : self_(o.self_) {}   // BUG: copies the pointer, shares the Model
    void releaseOnce() { self_->release(); }
    Concept* self_;
    // No destructor on purpose: deleting self_ from both copies would be a
    // real double delete (undefined behaviour). We call release() to show
    // the double free as a count, and accept the small leak of one Model.
};

// GOOD: made copyable through clone(). Each wrapper owns its own Model.
class Good {
public:
    template <class U>
    Good(U x) : self_(new Model<U>(std::move(x))) {}
    Good(const Good& o) : self_(o.self_->clone()) {}   // deep copy
    void releaseOnce() { self_->release(); }
    std::unique_ptr<Concept> self_;
};

int main() {
    std::cout << "bad -- copy shares one Model:\n";
    {
        Bad a = std::string("cfg");
        Bad b = a;            // copies the pointer
        a.releaseOnce();
        b.releaseOnce();      // same id released again -> double free
    }
    std::cout << "good -- copy clones the Model:\n";
    {
        Good a = std::string("cfg");
        Good b = a;           // clone(): b owns a distinct Model
        a.releaseOnce();
        b.releaseOnce();      // a different id -> each freed once
    }
    return 0;
}
