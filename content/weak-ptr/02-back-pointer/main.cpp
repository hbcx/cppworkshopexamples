#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// A parent OWNS its children (shared_ptr down the tree). Each child refers back
// to its parent with a weak_ptr, NOT a shared_ptr -- a shared_ptr both ways would
// form a cycle that never frees. The weak back-pointer models "knows its parent"
// without owning it.
struct Node {
    std::string name;
    std::weak_ptr<Node> parent;                  // up: non-owning
    std::vector<std::shared_ptr<Node>> children; // down: owning
    explicit Node(std::string n) : name(std::move(n)) {}
    ~Node() { std::cout << "~" << name << '\n'; }
};

static void addChild(const std::shared_ptr<Node>& parent, const std::shared_ptr<Node>& child) {
    child->parent = parent;                      // weak assignment: no cycle
    parent->children.push_back(child);           // shared: the parent owns the child
}

int main() {
    auto root = std::make_shared<Node>("root");
    auto leaf = std::make_shared<Node>("leaf");
    addChild(root, leaf);

    // A child can reach its parent while the parent is alive.
    if (std::shared_ptr<Node> p = leaf->parent.lock()) {
        std::cout << leaf->name << "'s parent is " << p->name << '\n';
    }
    std::cout << "root use_count = " << root.use_count() << '\n';   // 1: the weak back-edge did not raise it

    root.reset();                                // drop the tree's owner
    std::cout << "after root.reset, leaf's parent is "
              << (leaf->parent.expired() ? "gone" : "alive") << '\n';   // gone
    return 0;                                     // leaf destroyed here
}
