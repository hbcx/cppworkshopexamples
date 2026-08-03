#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// --- Approach 1: TRANSPARENT ------------------------------------------------
// add() lives on the Component, so a leaf and a composite share one interface and
// the client never needs to know which it holds. The cost: add() is meaningless
// on a leaf, so a leaf must reject it at RUNTIME.
struct TNode {
    virtual bool add(std::unique_ptr<TNode> child) = 0;   // may fail on a leaf
    virtual int leaves() const = 0;
    virtual ~TNode() = default;
};

class TFile : public TNode {
public:
    bool add(std::unique_ptr<TNode>) override {
        std::cout << "cannot add a child to a file\n";    // runtime rejection
        return false;
    }
    int leaves() const override { return 1; }
};

class TFolder : public TNode {
public:
    bool add(std::unique_ptr<TNode> child) override {
        children_.push_back(std::move(child));
        return true;
    }
    int leaves() const override {
        int n = 0;
        for (const auto& c : children_) n += c->leaves();
        return n;
    }
private:
    std::vector<std::unique_ptr<TNode>> children_;
};

// --- Approach 2: SAFE -------------------------------------------------------
// add() lives ONLY on the composite. A leaf has no add() at all, so misusing it
// is a COMPILE error, not a runtime failure -- but the client must hold the
// composite type to add, so leaf and composite are not fully interchangeable.
struct SNode {
    virtual int leaves() const = 0;
    virtual ~SNode() = default;
};

class SFile : public SNode {
public:
    int leaves() const override { return 1; }
};

class SFolder : public SNode {
public:
    void add(std::unique_ptr<SNode> child) { children_.push_back(std::move(child)); }  // only here
    int leaves() const override {
        int n = 0;
        for (const auto& c : children_) n += c->leaves();
        return n;
    }
private:
    std::vector<std::unique_ptr<SNode>> children_;
};

int main() {
    std::cout << "== transparent: one interface, rejection at runtime ==\n";
    auto folder = std::make_unique<TFolder>();
    folder->add(std::make_unique<TFile>());
    TFile leaf;
    leaf.add(std::make_unique<TFile>());          // compiles, but fails at runtime
    std::cout << "folder leaves: " << folder->leaves() << "\n";

    std::cout << "== safe: add only on the composite, checked at compile time ==\n";
    auto sfolder = std::make_unique<SFolder>();
    sfolder->add(std::make_unique<SFile>());
    sfolder->add(std::make_unique<SFile>());
    // SFile has no add(): writing sfile.add(...) would not compile at all -- the
    // misuse is impossible rather than merely handled.
    std::cout << "sfolder leaves: " << sfolder->leaves() << "\n";
    return 0;
}
