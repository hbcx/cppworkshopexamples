// Anti-pattern: state objects for behaviour, but transitions in the context.
//
// This one looks like State -- there are proper state (Stage) objects, each with
// its own behaviour -- so it is easy to believe the job is done. But the context
// still owns the transition graph: PipelineBad::step() inspects the current
// state's type and hardcodes what comes next. The states cannot tell you where
// they go, so every method that transitions repeats this chain, and adding a
// stage edits the context, not a stage. The fix is to let each state name its
// own successor (Stage::next), so the context only asks and swaps.

#include <iostream>
#include <memory>

// ---------- BAD: behaviour in the states, transitions in the context ----------
struct Stage {
    virtual ~Stage() = default;
    virtual void run() const = 0; // behaviour is correctly per-state...
    virtual const char* name() const = 0;
};
struct StageA : Stage { void run() const override { std::cout << "do A\n"; } const char* name() const override { return "A"; } };
struct StageB : Stage { void run() const override { std::cout << "do B\n"; } const char* name() const override { return "B"; } };
struct StageC : Stage { void run() const override { std::cout << "do C\n"; } const char* name() const override { return "C"; } };

class PipelineBad {
    std::unique_ptr<Stage> stage_{ new StageA() };
public:
    void step() {
        stage_->run();
        // ...but the transition graph is HERE, re-derived by inspecting the type.
        if      (dynamic_cast<StageA*>(stage_.get())) stage_.reset(new StageB());
        else if (dynamic_cast<StageB*>(stage_.get())) stage_.reset(new StageC());
        // Adding a stage edits this chain; every transitioning method needs it.
    }
    const char* name() const { return stage_->name(); }
};

// ---------- GOOD: each state names its own successor ----------
struct Step {
    virtual ~Step() = default;
    virtual void run() const = 0;
    virtual std::unique_ptr<Step> next() const = 0; // the state owns its transition
    virtual const char* name() const = 0;
};
struct StepC : Step {
    void run() const override { std::cout << "do C\n"; }
    std::unique_ptr<Step> next() const override { return nullptr; } // last stage
    const char* name() const override { return "C"; }
};
struct StepB : Step {
    void run() const override { std::cout << "do B\n"; }
    std::unique_ptr<Step> next() const override { return std::unique_ptr<Step>(new StepC()); }
    const char* name() const override { return "B"; }
};
struct StepA : Step {
    void run() const override { std::cout << "do A\n"; }
    std::unique_ptr<Step> next() const override { return std::unique_ptr<Step>(new StepB()); }
    const char* name() const override { return "A"; }
};

class PipelineGood {
    std::unique_ptr<Step> stage_{ new StepA() };
public:
    void step() {
        stage_->run();
        if (auto n = stage_->next()) stage_ = std::move(n); // just ask and swap
    }
    const char* name() const { return stage_->name(); }
};

int main() {
    std::cout << "--- bad ---\n";
    PipelineBad bad;
    for (int i = 0; i < 3; ++i) bad.step();
    std::cout << "end: " << bad.name() << "\n";

    std::cout << "--- good ---\n";
    PipelineGood good;
    for (int i = 0; i < 3; ++i) good.step();
    std::cout << "end: " << good.name() << "\n";
    return 0;
}
