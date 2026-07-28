#include <iostream>
#include <memory>
#include <vector>

// A Worker must hand a shared_ptr to ITSELF to a registry -- imagine registering
// a callback that may run after the caller has dropped its own reference.
// Deriving from enable_shared_from_this lets it produce a shared_ptr that shares
// the SAME control block, never a second one.
class Worker : public std::enable_shared_from_this<Worker> {
public:
    explicit Worker(int id) : id_(id) {}
    ~Worker() { std::cout << "Worker " << id_ << " destroyed\n"; }
    void registerInto(std::vector<std::shared_ptr<Worker>>& registry) {
        registry.push_back(shared_from_this());   // correct: same control block
    }
    int id() const { return id_; }
private:
    int id_;
};

int main() {
    std::vector<std::shared_ptr<Worker>> registry;
    {
        auto w = std::make_shared<Worker>(1);      // must already be shared-owned
        w->registerInto(registry);
        std::cout << "use_count after register = " << w.use_count() << '\n';   // 2
    }   // local w is gone, but the registry still owns Worker 1
    std::cout << "registry keeps it alive, use_count = " << registry[0].use_count() << '\n';  // 1
    std::cout << "still usable: Worker " << registry[0]->id() << '\n';
    registry.clear();   // last owner -> destroyed
    std::cout << "done\n";
    return 0;
}
