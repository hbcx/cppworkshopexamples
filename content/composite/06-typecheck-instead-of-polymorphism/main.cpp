#include <iostream>
#include <memory>
#include <utility>
#include <vector>

struct Node {
    virtual ~Node() = default;
};

class Leaf : public Node {
public:
    explicit Leaf(int v) : v_(v) {}
    int value() const { return v_; }
private:
    int v_;
};

class Branch : public Node {
public:
    void add(std::unique_ptr<Node> n) { kids_.push_back(std::move(n)); }
    const std::vector<std::unique_ptr<Node>>& kids() const { return kids_; }
private:
    std::vector<std::unique_ptr<Node>> kids_;
};

// ANTI-PATTERN: the operation lives OUTSIDE the nodes, in a free function that
// dynamic_casts each node to decide what to do. Every new operation repeats this
// type switch, and every new node type forces edits to every such function --
// and a node type nobody handled falls silently through the bottom. This is
// exactly the coupling Composite's uniform interface exists to remove.
int sumBad(const Node& n) {
    if (const auto* leaf = dynamic_cast<const Leaf*>(&n)) {
        return leaf->value();
    }
    if (const auto* branch = dynamic_cast<const Branch*>(&n)) {
        int total = 0;
        for (const auto& c : branch->kids()) total += sumBad(*c);
        return total;
    }
    return 0;   // a third node type would silently vanish here
}

// FIX: put the operation on the Component as a virtual function. Each node knows
// how to sum itself; adding a node type cannot break existing traversals, and
// there is no cast and no type switch anywhere.
struct GNode {
    virtual int sum() const = 0;
    virtual ~GNode() = default;
};

class GLeaf : public GNode {
public:
    explicit GLeaf(int v) : v_(v) {}
    int sum() const override { return v_; }
private:
    int v_;
};

class GBranch : public GNode {
public:
    void add(std::unique_ptr<GNode> n) { kids_.push_back(std::move(n)); }
    int sum() const override {
        int total = 0;
        for (const auto& c : kids_) total += c->sum();
        return total;
    }
private:
    std::vector<std::unique_ptr<GNode>> kids_;
};

int main() {
    auto root = std::make_unique<Branch>();
    root->add(std::make_unique<Leaf>(10));
    auto sub = std::make_unique<Branch>();
    sub->add(std::make_unique<Leaf>(20));
    sub->add(std::make_unique<Leaf>(30));
    root->add(std::move(sub));
    std::cout << "sum via dynamic_cast: " << sumBad(*root) << "\n";   // 60

    auto groot = std::make_unique<GBranch>();
    groot->add(std::make_unique<GLeaf>(10));
    auto gsub = std::make_unique<GBranch>();
    gsub->add(std::make_unique<GLeaf>(20));
    gsub->add(std::make_unique<GLeaf>(30));
    groot->add(std::move(gsub));
    std::cout << "sum via virtual call: " << groot->sum() << "\n";    // 60
    return 0;
}
