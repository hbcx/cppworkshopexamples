#include <iostream>
#include <memory>

// A stand-in for something that owns an OS resource: it announces its
// construction and destruction so you can see exactly when the unique_ptr
// releases it -- notice there is no delete anywhere in main().
class Resource {
public:
    explicit Resource(int id) : id_(id) {
        std::cout << "Resource " << id_ << " acquired\n";
    }
    ~Resource() {
        std::cout << "Resource " << id_ << " released\n";
    }
    void use() const {
        std::cout << "Resource " << id_ << " in use\n";
    }
private:
    int id_;
};

int main() {
    std::cout << "-- entering scope --\n";
    {
        std::unique_ptr<Resource> p(new Resource(1));
        p->use();                        // -> reaches through to the object
        (*p).use();                      // * dereferences to the same object
        std::cout << "p holds an object: " << (p ? "yes" : "no") << '\n';
    }   // p goes out of scope here: the Resource is deleted automatically
    std::cout << "-- left scope --\n";

    // reset() destroys the managed object right away, before the scope ends.
    std::unique_ptr<Resource> q(new Resource(2));
    q.reset();                           // Resource 2 released on this line
    std::cout << "after reset, q holds an object: " << (q ? "yes" : "no") << '\n';
    return 0;
}
