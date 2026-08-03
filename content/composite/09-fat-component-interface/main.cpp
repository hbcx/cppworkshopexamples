#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

// ANTI-PATTERN: to keep leaves and composites uniform, EVERY operation is piled
// onto the Component -- child management as well as the domain query -- so a leaf
// must implement a set of methods that make no sense for it (add, remove, child),
// each throwing or returning a lie. The interface is too wide, and each leaf
// carries dead, misleading code just to satisfy it.
struct FatNode {
    virtual int size() const = 0;
    virtual void add(std::unique_ptr<FatNode> child) = 0;
    virtual void remove(int index) = 0;
    virtual FatNode& child(int index) = 0;
    virtual ~FatNode() = default;
};

class FatFile : public FatNode {
public:
    explicit FatFile(int bytes) : bytes_(bytes) {}
    int size() const override { return bytes_; }
    void add(std::unique_ptr<FatNode>) override { throw std::logic_error("a file has no children"); }
    void remove(int) override { throw std::logic_error("a file has no children"); }
    FatNode& child(int) override { throw std::logic_error("a file has no children"); }
private:
    int bytes_;
};

// FIX: the Component carries ONLY the operations meaningful for every node
// (size). Child management lives on the composite, where it belongs, so a leaf
// implements nothing it cannot honour and there is no dead code to mislead.
struct Node {
    virtual int size() const = 0;
    virtual ~Node() = default;
};

class File : public Node {
public:
    explicit File(int bytes) : bytes_(bytes) {}
    int size() const override { return bytes_; }
private:
    int bytes_;
};

class Folder : public Node {
public:
    void add(std::unique_ptr<Node> child) { kids_.push_back(std::move(child)); }   // only on the composite
    int size() const override {
        int total = 0;
        for (const auto& c : kids_) total += c->size();
        return total;
    }
private:
    std::vector<std::unique_ptr<Node>> kids_;
};

int main() {
    std::cout << "== fat interface: the leaf carries methods it cannot honour ==\n";
    FatFile fat(100);
    std::cout << "fat file size: " << fat.size() << "\n";
    try {
        fat.add(std::make_unique<FatFile>(1));
    } catch (const std::logic_error& e) {
        std::cout << "fat file add() throws: " << e.what() << "\n";
    }

    std::cout << "== lean interface: the leaf implements only what it can ==\n";
    auto folder = std::make_unique<Folder>();
    folder->add(std::make_unique<File>(30));
    folder->add(std::make_unique<File>(70));
    std::cout << "folder size: " << folder->size() << "\n";   // 100
    return 0;
}
