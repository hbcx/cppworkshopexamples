// Anti-pattern: a mandatory step exposed as a hook, then overridden to nothing.
//
// Not everything a skeleton does should be a customisation point. If a step
// enforces an invariant -- validation, a security check, a required cleanup --
// exposing it as a virtual lets a subclass override it to a no-op and quietly
// disable it. Here PipelineBad makes validate() virtual, and a subclass turns it
// off, so out-of-range data flows straight through. The fix is to keep truly
// mandatory steps OUT of the overridable surface: run them from the non-virtual
// skeleton itself, and expose only the parts that are genuinely meant to vary.

#include <iostream>

// ---------- BAD: the invariant-enforcing step is virtual, so it can be skipped ----------
class PipelineBad {
public:
    virtual ~PipelineBad() = default;
    void run(int value) {
        if (!validate(value)) { std::cout << "rejected " << value << "\n"; return; }
        transform(value);
    }
protected:
    virtual bool validate(int v) { return v >= 0; }        // meant to be mandatory...
    virtual void transform(int v) { std::cout << "out " << v << "\n"; }
};

class TrustingPipeline : public PipelineBad {
protected:
    bool validate(int) override { return true; }           // ...but overridden to a no-op
    void transform(int v) override { std::cout << "out " << v << "\n"; }
};

// ---------- GOOD: the mandatory step is baked into the skeleton; only the core varies ----------
class PipelineGood {
public:
    virtual ~PipelineGood() = default;
    void run(int value) {
        if (value < 0) { std::cout << "rejected " << value << "\n"; return; } // fixed, non-virtual
        transform(value);   // the only extension point
    }
protected:
    virtual void transform(int v) = 0;
};

class DoublingPipeline : public PipelineGood {
protected:
    void transform(int v) override { std::cout << "out " << (v * 2) << "\n"; }
};

int main() {
    std::cout << "--- bad (validation overridden away) ---\n";
    TrustingPipeline{}.run(-5);   // out -5   <- invalid value passed the disabled check

    std::cout << "--- good (validation not overridable) ---\n";
    DoublingPipeline{}.run(-5);   // rejected -5
    DoublingPipeline{}.run(3);    // out 6
    return 0;
}
