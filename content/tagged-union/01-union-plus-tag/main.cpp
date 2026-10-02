#include <iostream>

// A tagged union: a union that can hold one of several types, plus a tag that
// says which one is currently active. Only the tagged member may be read.
struct Number {
    enum class Kind { Int, Double } tag;
    union {
        int    i;
        double d;
    };
};

Number makeInt(int v)      { Number n; n.tag = Number::Kind::Int;    n.i = v; return n; }
Number makeDouble(double v){ Number n; n.tag = Number::Kind::Double; n.d = v; return n; }

// Read the value by checking the tag first -- that is what the tag is for.
void print(const Number& n) {
    switch (n.tag) {
        case Number::Kind::Int:    std::cout << "int " << n.i << '\n';    break;
        case Number::Kind::Double: std::cout << "double " << n.d << '\n'; break;
    }
}

int main() {
    Number a = makeInt(42);
    Number b = makeDouble(3.5);
    print(a);   // int 42
    print(b);   // double 3.5

    // The union stores ONE member at a time -- both share the same storage.
    std::cout << "sizeof(Number) = " << sizeof(Number)
              << " (tag + the largest member, not the sum)\n";
    return 0;
}
