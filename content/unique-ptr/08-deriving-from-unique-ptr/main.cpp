#include <iostream>
#include <memory>
#include <string>

// A C-style resource API (imagine fopen/fclose, or a database open/close).
struct Resource {
    std::string name;
};

static Resource* res_open(const std::string& name) {
    std::cout << "open " << name << '\n';
    return new Resource{name};
}

static void res_close(Resource* r) {
    std::cout << "close " << r->name << '\n';
    delete r;
}

// A stateless deleter that calls the C free function.
struct ResDeleter {
    void operator()(Resource* r) const { res_close(r); }
};

// Publicly deriving from unique_ptr bakes the deleter into a NAMED type and lets
// us add a domain-specific constructor, so callers write ResHandle("cfg")
// instead of std::unique_ptr<Resource, ResDeleter>(res_open("cfg")). The deleter
// is part of the type, so it can never be forgotten.
//
// Keep it stateless (add no owning members) and never delete a ResHandle through
// a unique_ptr* base pointer: std::unique_ptr has no virtual destructor.
class ResHandle : public std::unique_ptr<Resource, ResDeleter> {
    using Base = std::unique_ptr<Resource, ResDeleter>;
public:
    using Base::Base;                    // inherit unique_ptr's constructors
    // The ergonomic addition: open a resource by name.
    explicit ResHandle(const std::string& name) : Base(res_open(name)) {}
};

int main() {
    ResHandle a("config");               // opens; closed automatically at scope end
    std::cout << "using " << a->name << '\n';   // -> comes from the base

    ResHandle b = std::move(a);          // still move-only: ownership transfers
    std::cout << "a holds: " << (a ? "yes" : "no")
              << ", b holds: " << (b ? "yes" : "no") << '\n';

    // A stateless deleter plus no extra members means no size overhead:
    std::cout << "sizeof(ResHandle) == one pointer? "
              << (sizeof(ResHandle) == sizeof(void*) ? "yes" : "no") << '\n';
    return 0;                            // b closes the resource exactly once
}
