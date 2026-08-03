#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// ANTI-PATTERN: add() is on the Component (a transparent design), but the leaf
// implements it as a silent no-op. Adding a child to a leaf compiles, runs, and
// reports nothing -- the child is quietly dropped, so a caller believes the tree
// holds it when it does not. The failure is neither prevented nor reported.
struct BadNode {
    virtual void add(std::unique_ptr<BadNode> child) = 0;
    virtual int leaves() const = 0;
    virtual ~BadNode() = default;
};

class BadFile : public BadNode {
public:
    void add(std::unique_ptr<BadNode>) override { /* silently ignored -- BUG */ }
    int leaves() const override { return 1; }
};

class BadFolder : public BadNode {
public:
    void add(std::unique_ptr<BadNode> child) override { kids_.push_back(std::move(child)); }
    int leaves() const override {
        int n = 0;
        for (const auto& c : kids_) n += c->leaves();
        return n;
    }
private:
    std::vector<std::unique_ptr<BadNode>> kids_;
};

// FIX: in a transparent design a leaf must REJECT the operation visibly -- here
// by returning false so the caller can react (throwing is the other option). The
// impossible action is now reported, not swallowed. (The safe alternative is to
// keep add() off the Component entirely, as an earlier example shows.)
struct GoodNode {
    virtual bool add(std::unique_ptr<GoodNode> child) = 0;
    virtual int leaves() const = 0;
    virtual ~GoodNode() = default;
};

class GoodFile : public GoodNode {
public:
    bool add(std::unique_ptr<GoodNode>) override { return false; }   // explicit failure
    int leaves() const override { return 1; }
};

class GoodFolder : public GoodNode {
public:
    bool add(std::unique_ptr<GoodNode> child) override {
        kids_.push_back(std::move(child));
        return true;
    }
    int leaves() const override {
        int n = 0;
        for (const auto& c : kids_) n += c->leaves();
        return n;
    }
private:
    std::vector<std::unique_ptr<GoodNode>> kids_;
};

int main() {
    std::cout << "== silent no-op leaf ==\n";
    BadFile bad;
    bad.add(std::make_unique<BadFile>());        // looks like it worked
    std::cout << "leaves after adding to a file: " << bad.leaves()
              << " (the child was dropped, with no error)\n";

    std::cout << "== explicit rejection ==\n";
    GoodFile good;
    if (!good.add(std::make_unique<GoodFile>())) {
        std::cout << "add to a file refused -- the caller can handle it\n";
    }
    return 0;
}
