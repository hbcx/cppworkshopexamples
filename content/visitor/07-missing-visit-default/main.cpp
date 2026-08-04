// Anti-pattern: a do-nothing default visit hides a forgotten type.
//
// If the visitor base gives visit a non-pure default (an empty body), a concrete
// visitor that forgets to handle a type inherits that default and silently
// ignores the type. Adding a Triangle and forgetting it in one visitor produces
// no error and no output for triangles. The fix is a pure virtual visit per
// type: every visitor must then handle every type, so a forgotten one is a
// compile error. (The BAD and GOOD hierarchies are separated by a B/G prefix.)

#include <iostream>

// ================= BAD: defaults let a forgotten type pass silently =========
class BCircle; class BSquare; class BTriangle;
class BVisitor {
public:
    virtual ~BVisitor() = default;
    virtual void visit(const BCircle&) {}   // non-pure defaults: forgetting is silent
    virtual void visit(const BSquare&) {}
    virtual void visit(const BTriangle&) {}
};
class BShape { public: virtual ~BShape() = default; virtual void accept(BVisitor&) const = 0; };
class BCircle : public BShape { public: void accept(BVisitor& v) const override { v.visit(*this); } };
class BSquare : public BShape { public: void accept(BVisitor& v) const override { v.visit(*this); } };
class BTriangle : public BShape { public: void accept(BVisitor& v) const override { v.visit(*this); } };
class BArea : public BVisitor {
public:
    void visit(const BCircle&) override { std::cout << "  circle area\n"; }
    void visit(const BSquare&) override { std::cout << "  square area\n"; }
    // FORGOT BTriangle -> inherits the empty default -> triangles silently ignored
};

// ================= GOOD: pure virtuals force every type to be handled ========
class GCircle; class GSquare; class GTriangle;
class GVisitor {
public:
    virtual ~GVisitor() = default;
    virtual void visit(const GCircle&) = 0; // pure: a visitor must handle all three
    virtual void visit(const GSquare&) = 0;
    virtual void visit(const GTriangle&) = 0;
};
class GShape { public: virtual ~GShape() = default; virtual void accept(GVisitor&) const = 0; };
class GCircle : public GShape { public: void accept(GVisitor& v) const override { v.visit(*this); } };
class GSquare : public GShape { public: void accept(GVisitor& v) const override { v.visit(*this); } };
class GTriangle : public GShape { public: void accept(GVisitor& v) const override { v.visit(*this); } };
class GArea : public GVisitor {
public:
    void visit(const GCircle&) override { std::cout << "  circle area\n"; }
    void visit(const GSquare&) override { std::cout << "  square area\n"; }
    void visit(const GTriangle&) override { std::cout << "  triangle area\n"; } // required to compile
};

int main() {
    BCircle bc; BSquare bs; BTriangle bt;
    const BShape* bad[] = { &bc, &bs, &bt };
    std::cout << "bad (default visit): triangle is silently skipped\n";
    BArea barea;
    for (const BShape* s : bad) s->accept(barea);

    GCircle gc; GSquare gs; GTriangle gt;
    const GShape* good[] = { &gc, &gs, &gt };
    std::cout << "good (pure visit): every type handled\n";
    GArea garea;
    for (const GShape* s : good) s->accept(garea);
    return 0;
}
