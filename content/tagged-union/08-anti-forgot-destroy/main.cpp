#include <iostream>
#include <memory>
#include <new>

struct Resource {
    int id;
    explicit Resource(int i) : id(i) { ++live; }
    ~Resource() { --live; }
    static int live;
};
int Resource::live = 0;

class Value {
    enum class Kind { Int, Res } tag_;
    union {
        int      i_;
        Resource r_;
    };
public:
    explicit Value(int v) : tag_(Kind::Int) { i_ = v; }
    ~Value() { if (tag_ == Kind::Res) std::destroy_at(&r_); }
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;

    // FIXED: destroy the current active member before constructing the new one.
    void setResource(int id) {
        if (tag_ == Kind::Res) std::destroy_at(&r_);   // <-- the fix
        ::new (&r_) Resource(id);
        tag_ = Kind::Res;
    }

    // BUGGY: builds the new Resource over the old bytes without destroying the
    // old active member, so a Resource that was already active leaks.
    void setResource_buggy(int id) {
        ::new (&r_) Resource(id);
        tag_ = Kind::Res;
    }
};

int main() {
    {
        Value v{0};
        v.setResource(1);   // Res active (id 1)
        v.setResource(2);   // the fix destroys id 1 first, then builds id 2
    }   // ~Value destroys id 2
    std::cout << "with the fix, live Resources: " << Resource::live << '\n';   // 0

    Resource::live = 0;
    {
        Value v{0};
        v.setResource_buggy(1);   // Res active (id 1)
        v.setResource_buggy(2);   // overwrites id 1 without destroying it -> leak
    }   // ~Value destroys only id 2
    std::cout << "with the bug, live Resources: " << Resource::live << '\n';   // 1 (leaked)
    return 0;
}
