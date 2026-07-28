#include <iostream>
#include <memory>

// The object announces its lifetime so you can see that it is destroyed only
// when the LAST owner goes away, not the first.
class Resource {
public:
    explicit Resource(int id) : id_(id) { std::cout << "Resource " << id_ << " acquired\n"; }
    ~Resource() { std::cout << "Resource " << id_ << " released\n"; }
    int id() const { return id_; }
private:
    int id_;
};

int main() {
    std::shared_ptr<Resource> a(new Resource(1));
    std::cout << "one owner, use_count = " << a.use_count() << '\n';   // 1
    {
        std::shared_ptr<Resource> b = a;    // copy: shares ownership, count -> 2
        std::cout << "two owners, use_count = " << a.use_count() << '\n';   // 2
        std::cout << "b sees Resource " << b->id() << '\n';
    }   // b is destroyed: count -> 1, but the Resource is NOT released yet
    std::cout << "after inner scope, use_count = " << a.use_count() << '\n'; // 1
    std::cout << "a still sees Resource " << a->id() << '\n';
    return 0;   // a is destroyed: count -> 0, the Resource is released here
}
