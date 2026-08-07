#include <iostream>
#include <memory>
#include <utility>
#include <vector>

int heapModels = 0;   // type-erased values that allocated a Model on the heap

struct Concept {
    virtual ~Concept() = default;
    virtual int value() const = 0;
};
template <class T>
struct Model : Concept {
    Model(T x) : data_(std::move(x)) { ++heapModels; }
    int value() const override { return static_cast<int>(data_); }
    T data_;
};
class AnyValue {
public:
    template <class T>
    AnyValue(T x) : self_(new Model<T>(std::move(x))) {}
    int value() const { return self_->value(); }
private:
    std::unique_ptr<Concept> self_;
};

// BAD: the element type is always int, known at compile time, yet each
// element is boxed behind a virtual call and a heap allocation.
int sumErased(const std::vector<AnyValue>& xs) {
    int s = 0;
    for (const AnyValue& x : xs) s += x.value();   // virtual dispatch per element
    return s;
}

// GOOD: a template (here just std::vector<int>) keeps the type. No Model,
// no heap box, no virtual call -- the loop inlines.
template <class T>
int sumDirect(const std::vector<T>& xs) {
    int s = 0;
    for (const T& x : xs) s += x;
    return s;
}

int main() {
    std::vector<AnyValue> erased;
    erased.push_back(1);
    erased.push_back(2);
    erased.push_back(3);
    std::cout << "erased sum=" << sumErased(erased)
              << "  heap Models=" << heapModels << "\n";

    std::vector<int> direct;
    direct.push_back(1);
    direct.push_back(2);
    direct.push_back(3);
    std::cout << "direct sum=" << sumDirect(direct)
              << "  heap Models=" << heapModels << "  <- no new boxes\n";

    // The type was int all along. Type erasure bought nothing here and
    // cost a heap allocation and a virtual call per element. The sign: an
    // erased wrapper whose every construction site passes the same type.
    return 0;
}
