#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Flyweight: the INTRINSIC state -- shared, context-independent, immutable. A
// tree's species carries its name and, in a real engine, its mesh and textures:
// megabytes that must not be duplicated once per tree.
class TreeType {
public:
    explicit TreeType(std::string name) : name_(std::move(name)) { ++built; }
    void render(int x, int y) const {
        std::cout << name_ << " at (" << x << "," << y << ")\n";
    }
    static int built;
private:
    std::string name_;
};
int TreeType::built = 0;

// A forest of many trees drawn from few species.
class Forest {
public:
    void plant(const std::string& species, int x, int y) {
        auto& type = types_[species];                 // intern the species...
        if (!type) type = std::make_shared<TreeType>(species);
        trees_.push_back({type, x, y});               // ...and store only a pointer to it
    }
    void render() const {
        for (const auto& t : trees_) t.type->render(t.x, t.y);
    }
    std::size_t treeCount() const { return trees_.size(); }
private:
    struct Tree {                                     // the EXTRINSIC state: position,
        std::shared_ptr<TreeType> type;               // plus a pointer to the shared
        int x;                                        // intrinsic state
        int y;
    };
    std::map<std::string, std::shared_ptr<TreeType>> types_;
    std::vector<Tree> trees_;
};

int main() {
    Forest forest;
    const char* species[] = {"oak", "pine", "oak", "pine", "oak", "birch"};
    int i = 0;
    for (const auto* s : species) {
        forest.plant(s, i, i * 2);
        ++i;
    }

    forest.render();
    std::cout << forest.treeCount() << " trees, but only " << TreeType::built
              << " tree types built\n";
    return 0;
}
