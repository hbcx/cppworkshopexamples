// Anti-pattern: the shallow-copy double free.
//
// A pimpl that owns its Impl through a raw pointer and lets the compiler copy it
// gets a SHALLOW copy: both objects hold the same pointer and both delete it --
// a double free (undefined behaviour). We describe it; we do NOT run it.
//
// The fix: own the Impl with a unique_ptr (move-only by default, so a copy is a
// compile error), and add a deep copy if the type is meant to be a value.

#include <iostream>
#include <memory>
#include <string>

// ---------- raw owning pointer + the compiler's shallow copy ----------
namespace bad {

struct Impl {
    std::string data;
};

class Handle {
public:
    explicit Handle(std::string d) : impl_(new Impl{std::move(d)}) {}
    ~Handle() { delete impl_; }
    // No copy constructor declared. The implicit one copies the POINTER, so:
    //     bad::Handle b = a;   // two owners of one Impl -> double free at
    //                          // scope exit (UB). Never done here.
    std::string data() const { return impl_->data; }

private:
    Impl* impl_;
};

} // namespace bad

// ---------- unique_ptr ownership + an explicit deep copy ----------
namespace good {

struct Impl {
    std::string data;
};

class Handle {
public:
    explicit Handle(std::string d) : impl_(new Impl{std::move(d)}) {}
    Handle(const Handle& other) : impl_(new Impl(*other.impl_)) {} // deep copy
    Handle& operator=(const Handle& other) {
        if (this != &other) *impl_ = *other.impl_;
        return *this;
    }
    std::string data() const { return impl_->data; }
    void setData(const std::string& d) { impl_->data = d; }

private:
    std::unique_ptr<Impl> impl_;
};

} // namespace good

int main() {
    bad::Handle a("shared");
    std::cout << "[bad]  data=" << a.data()
              << " (copying it would double-free -- not done)\n";

    good::Handle x("original");
    good::Handle y = x;      // deep copy: y owns its own Impl
    y.setData("changed");
    std::cout << "[good] x=" << x.data() << " y=" << y.data() << "\n";
}
