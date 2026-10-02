#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <utility>

// A tagged union done correctly: copy, move and destroy all switch on the tag to
// operate on the ACTIVE member. This is what std::variant does for you.
class Value {
    enum class Kind { Int, Str } tag_;
    union {
        int         i_;
        std::string s_;
    };
    void destroy() { if (tag_ == Kind::Str) std::destroy_at(&s_); }
    void construct_from(const Value& o) {
        tag_ = o.tag_;
        if (tag_ == Kind::Int) i_ = o.i_;
        else ::new (&s_) std::string(o.s_);
    }
    void construct_from(Value&& o) {
        tag_ = o.tag_;
        if (tag_ == Kind::Int) i_ = o.i_;
        else ::new (&s_) std::string(std::move(o.s_));
    }
public:
    explicit Value(int v)         : tag_(Kind::Int) { i_ = v; }
    explicit Value(std::string v) : tag_(Kind::Str) { ::new (&s_) std::string(std::move(v)); }
    ~Value() { destroy(); }

    Value(const Value& o) { construct_from(o); }
    Value(Value&& o)      { construct_from(std::move(o)); }
    Value& operator=(const Value& o) {
        if (this != &o) { destroy(); construct_from(o); }
        return *this;
    }
    Value& operator=(Value&& o) {
        if (this != &o) { destroy(); construct_from(std::move(o)); }
        return *this;
    }

    void print() const {
        if (tag_ == Kind::Int) std::cout << "int " << i_ << '\n';
        else                   std::cout << "string \"" << s_ << "\"\n";
    }
};

int main() {
    Value a{std::string("alpha")};
    Value b = a;              // copy: builds b's string from a's
    b.print();                // string "alpha"

    Value c{10};
    c = a;                    // assign: destroys c's int, copies a's string
    c.print();                // string "alpha"

    Value d = std::move(a);   // move: steals a's string buffer
    d.print();                // string "alpha"

    a = Value{99};            // a becomes an int again (its moved-from string is destroyed)
    a.print();                // int 99
    return 0;
}
