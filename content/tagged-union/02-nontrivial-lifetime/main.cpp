#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <utility>

// A tagged union with a NON-TRIVIAL member (std::string). A union does not
// construct or destroy its members for you, so we placement-new the active
// member and destroy it by hand.
class Value {
    enum class Kind { Int, Str } tag_;
    union {
        int         i_;
        std::string s_;
    };
public:
    explicit Value(int v)         : tag_(Kind::Int) { i_ = v; }
    explicit Value(std::string v) : tag_(Kind::Str) { ::new (&s_) std::string(std::move(v)); }

    ~Value() {
        if (tag_ == Kind::Str) std::destroy_at(&s_);   // destroy the active member
    }
    Value(const Value&) = delete;             // copy/move are covered in example 03
    Value& operator=(const Value&) = delete;

    void print() const {
        if (tag_ == Kind::Int) std::cout << "int " << i_ << '\n';
        else                   std::cout << "string \"" << s_ << "\"\n";
    }
};

int main() {
    Value a{7};
    Value b{std::string("hello")};
    a.print();   // int 7
    b.print();   // string "hello"

    // b's std::string is built with placement new and destroyed in ~Value.
    // Without those, its buffer would never be constructed or freed.
    return 0;
}
