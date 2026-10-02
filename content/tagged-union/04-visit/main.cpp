#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <utility>

class Value {
    enum class Kind { Int, Str } tag_;
    union {
        int         i_;
        std::string s_;
    };
public:
    explicit Value(int v)         : tag_(Kind::Int) { i_ = v; }
    explicit Value(std::string v) : tag_(Kind::Str) { ::new (&s_) std::string(std::move(v)); }
    ~Value() { if (tag_ == Kind::Str) std::destroy_at(&s_); }
    Value(const Value&) = delete;
    Value& operator=(const Value&) = delete;

    // Call f with the ACTIVE member, whatever its type. The switch guarantees f
    // only ever sees the member that is really there -- this is what std::visit
    // does, minus the compile-time exhaustiveness check.
    template <class F>
    void visit(F&& f) const {
        switch (tag_) {
            case Kind::Int: f(i_); break;
            case Kind::Str: f(s_); break;
        }
    }
};

int main() {
    Value a{42};
    Value b{std::string("hi")};

    // One visitor handles every alternative: a generic lambda prints whatever it
    // is handed, be it the int or the string.
    auto show = [](const auto& x) { std::cout << "value: " << x << '\n'; };
    a.visit(show);   // value: 42
    b.visit(show);   // value: hi
    return 0;
}
