#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Component: the common interface for both a single item (a leaf) and a group of
// items (a composite). A client talks only to this and never asks which it holds.
struct Node {
    virtual int size() const = 0;              // bytes
    virtual void print(int indent) const = 0;
    virtual ~Node() = default;
};

static void pad(int n) {
    for (int i = 0; i < n; ++i) std::cout << ' ';
}

// Leaf: a file has no children; its size is simply its own.
class File : public Node {
public:
    File(std::string name, int bytes) : name_(std::move(name)), bytes_(bytes) {}
    int size() const override { return bytes_; }
    void print(int indent) const override {
        pad(indent);
        std::cout << name_ << " (" << bytes_ << ")\n";
    }
private:
    std::string name_;
    int bytes_;
};

// Composite: a directory holds children (files or other directories) and defines
// each operation in terms of its children's -- its size is the sum of theirs, its
// print recurses into theirs. The recursion lives here, once.
class Directory : public Node {
public:
    explicit Directory(std::string name) : name_(std::move(name)) {}
    void add(std::unique_ptr<Node> child) { children_.push_back(std::move(child)); }
    int size() const override {
        int total = 0;
        for (const auto& c : children_) total += c->size();   // same call on every child
        return total;
    }
    void print(int indent) const override {
        pad(indent);
        std::cout << name_ << "/\n";
        for (const auto& c : children_) c->print(indent + 2);
    }
private:
    std::string name_;
    std::vector<std::unique_ptr<Node>> children_;
};

int main() {
    auto root = std::make_unique<Directory>("root");
    root->add(std::make_unique<File>("a.txt", 100));

    auto sub = std::make_unique<Directory>("sub");
    sub->add(std::make_unique<File>("b.txt", 30));
    sub->add(std::make_unique<File>("c.txt", 70));
    root->add(std::move(sub));

    root->print(0);
    // The client asks the root for its size and does not care that the answer
    // comes from a whole tree -- a leaf and a composite answer the same call.
    std::cout << "total size: " << root->size() << " bytes\n";   // 200
    return 0;
}
