// Anti-pattern: global state instead of a passed Context.
//
// The Context exists so interpretation is a pure function of (tree, inputs):
// pass the bindings in, and the same tree evaluates independently against any
// inputs. Put the bindings in a global instead, and interpretation depends on
// hidden mutable state: two evaluations that should be independent share it, so
// one that forgets to set (or reset) a variable silently reads another's leftover
// value -- and the interpreter cannot be used from two places at once. The fix is
// to pass a Context.

#include <iostream>
#include <string>
#include <map>
#include <utility>

// ---- BAD: variable bindings live in a global the nodes read ----
std::map<std::string, int> g_vars;
class VarBad {
    std::string name_;
public:
    explicit VarBad(std::string n) : name_(std::move(n)) {}
    int eval() const {
        std::map<std::string, int>::const_iterator it = g_vars.find(name_);
        return it != g_vars.end() ? it->second : 0; // reads whatever the global holds
    }
};

// ---- GOOD: bindings are passed in a Context ----
class Context {
    std::map<std::string, int> vars_;
public:
    void set(const std::string& k, int v) { vars_[k] = v; }
    int get(const std::string& k) const {
        std::map<std::string, int>::const_iterator it = vars_.find(k);
        return it != vars_.end() ? it->second : 0;
    }
};
class VarGood {
    std::string name_;
public:
    explicit VarGood(std::string n) : name_(std::move(n)) {}
    int eval(const Context& ctx) const { return ctx.get(name_); }
};

int main() {
    VarBad x("x");
    // First evaluation: x should be 10.
    g_vars["x"] = 10;
    std::cout << "bad first  (x=10): " << x.eval() << "\n"; // 10
    // Second, independent evaluation: x should be 5 here, but the code updated a
    // different variable and forgot to set x -- so it reads the STALE global 10.
    g_vars["y"] = 5;
    std::cout << "bad second (x=5?): " << x.eval() << "\n"; // 10, wrong -- shared global

    VarGood gx("x");
    Context c1; c1.set("x", 10);
    Context c2; c2.set("x", 5);
    std::cout << "good first  (x=10): " << gx.eval(c1) << "\n"; // 10
    std::cout << "good second (x=5):  " << gx.eval(c2) << "\n"; // 5, independent
    return 0;
}
