#include <iostream>
#include <utility>

// The real subject: an ordinary object with member functions.
class Service {
public:
    void ping() const { std::cout << "service: ping\n"; }
    void status() const { std::cout << "service: ok\n"; }
};

// A smart-reference proxy. It holds the real object and overloads operator-> so
// that every member access through it is intercepted -- here counted and logged
// -- while the call site still reads exactly like using a pointer (svc->ping()).
// This is Proxy realised through operator->; std::shared_ptr is the same idea in
// the standard library, a proxy that reference-counts on copy and destruction.
template <class T>
class AccessLogged {
public:
    explicit AccessLogged(T obj) : obj_(std::move(obj)) {}

    // operator-> runs first, so it can bracket the access, then returns a pointer
    // the compiler uses to complete the member call.
    T* operator->() {
        ++count_;
        std::cout << "[access " << count_ << "] ";
        return &obj_;
    }
    int accesses() const { return count_; }
private:
    T obj_;
    int count_ = 0;
};

int main() {
    AccessLogged<Service> svc{Service{}};
    svc->ping();       // [access 1] service: ping
    svc->status();     // [access 2] service: ok
    svc->ping();       // [access 3] service: ping
    std::cout << "total accesses: " << svc.accesses() << "\n";
    return 0;
}
