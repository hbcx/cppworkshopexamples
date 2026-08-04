// Anti-pattern: no handling of invalid input.
//
// An interpreter runs on external input, so it must decide what an INVALID
// sentence does: an unknown variable, a division by zero, a malformed
// expression. Silently defaulting -- an unknown variable reads as 0, a bad token
// parses as nothing -- produces a wrong answer with no error, and a real
// undefined operation (integer division by zero) is undefined behaviour. The fix
// is to detect the error and report it. Here the Context records errors; the BAD
// path ignores that signal, the GOOD path checks it. The divide-by-zero UB is
// described and not run.

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <utility>

class Context {
    std::map<std::string, int> vars_;
    mutable std::string error_; // set when interpretation hits invalid input
public:
    void set(const std::string& k, int v) { vars_[k] = v; }
    int get(const std::string& k) const {
        std::map<std::string, int>::const_iterator it = vars_.find(k);
        if (it == vars_.end()) { setError("unknown variable " + k); return 0; }
        return it->second;
    }
    void setError(const std::string& msg) const { if (error_.empty()) error_ = msg; }
    bool ok() const { return error_.empty(); }
    const std::string& error() const { return error_; }
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual int interpret(const Context& ctx) const = 0;
};
class Number : public Expr {
    int v_;
public:
    explicit Number(int v) : v_(v) {}
    int interpret(const Context&) const override { return v_; }
};
class Var : public Expr {
    std::string name_;
public:
    explicit Var(std::string n) : name_(std::move(n)) {}
    int interpret(const Context& ctx) const override { return ctx.get(name_); }
};
class Add : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Add(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret(const Context& ctx) const override { return l_->interpret(ctx) + r_->interpret(ctx); }
};
class Div : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Div(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret(const Context& ctx) const override {
        int a = l_->interpret(ctx), b = r_->interpret(ctx);
        if (b == 0) { ctx.setError("division by zero"); return 0; } // checked
        // BAD would be `return a / b;` here -- integer division by zero is
        // UNDEFINED BEHAVIOUR (it may crash or trap), which is why it is checked.
        return a / b;
    }
};

int main() {
    using E = std::unique_ptr<Expr>;

    // Expression: a + z, where z is not bound.
    E expr(new Add(E(new Var("a")), E(new Var("z"))));
    Context ctx; ctx.set("a", 5);
    int result = expr->interpret(ctx);
    // BAD: ignore the error signal and use the result -> silently wrong (z was 0).
    std::cout << "bad (ignored error):  " << result << "\n"; // 5
    // GOOD: check the context for an error.
    if (!ctx.ok())
        std::cout << "good (checked):       error: " << ctx.error() << "\n";

    // Expression: a / b, with b == 0.
    E expr2(new Div(E(new Var("a")), E(new Var("b"))));
    Context ctx2; ctx2.set("a", 10); ctx2.set("b", 0);
    (void)expr2->interpret(ctx2);
    if (!ctx2.ok())
        std::cout << "good (checked div):   error: " << ctx2.error() << "\n";
    return 0;
}
