// Anti-pattern: one fat interpret() switching on a node-kind tag.
//
// The tree can be built without the pattern: a single Node struct with a kind
// tag, interpreted by one function that switches on the tag. It works and is even
// compact, but it is the enum-and-switch design the pattern replaces -- adding a
// node kind edits the switch, adding a SECOND operation (print, optimise)
// duplicates the whole switch elsewhere, and a forgotten case falls through to a
// silent default. A class per rule with a virtual interpret puts each kind's
// logic in one place and lets the compiler enforce completeness.

#include <iostream>
#include <memory>
#include <utility>

// ---- BAD: a tagged struct interpreted by a switch ----
struct Node {
    enum Kind { Num, Add, Mul } kind;
    int value;      // for Num
    const Node* l;  // for Add/Mul
    const Node* r;
};
int interpretBad(const Node* n) {
    switch (n->kind) {
        case Node::Num: return n->value;
        case Node::Add: return interpretBad(n->l) + interpretBad(n->r);
        case Node::Mul: return interpretBad(n->l) * interpretBad(n->r);
    }
    return 0; // a new kind not added here falls through silently
}

// ---- GOOD: a class per rule, each interpreting itself ----
class Expr {
public:
    virtual ~Expr() = default;
    virtual int interpret() const = 0;
};
class Number : public Expr {
    int v_;
public:
    explicit Number(int v) : v_(v) {}
    int interpret() const override { return v_; }
};
class Add : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Add(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override { return l_->interpret() + r_->interpret(); }
};
class Mul : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Mul(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override { return l_->interpret() * r_->interpret(); }
};

int main() {
    // (2 + 3) * 4
    Node two{ Node::Num, 2, nullptr, nullptr };
    Node three{ Node::Num, 3, nullptr, nullptr };
    Node four{ Node::Num, 4, nullptr, nullptr };
    Node add{ Node::Add, 0, &two, &three };
    Node root{ Node::Mul, 0, &add, &four };
    std::cout << "bad:  " << interpretBad(&root) << "\n"; // 20

    std::unique_ptr<Expr> tree(new Mul(
        std::unique_ptr<Expr>(new Add(
            std::unique_ptr<Expr>(new Number(2)),
            std::unique_ptr<Expr>(new Number(3)))),
        std::unique_ptr<Expr>(new Number(4))));
    std::cout << "good: " << tree->interpret() << "\n"; // 20
    return 0;
}
