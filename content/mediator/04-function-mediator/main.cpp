// The lightweight Mediator: a std::function event bus.
//
// When colleagues are not a fixed hierarchy of classes but independent modules
// that come and go, the mediator can be a small event bus: subscribers register
// a callback for a topic, and a publisher emits a topic with a payload without
// knowing who -- if anyone -- listens. This is the loosest, most decoupled form
// of Mediator; its cost is that the coupling moves into the string topic names,
// which are not checked by the compiler (the seed of a later anti-pattern).

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

class EventBus { // the mediator: a hub of topic -> callbacks
    std::unordered_map<std::string, std::vector<std::function<void(const std::string&)>>> subs_;
public:
    void subscribe(const std::string& topic, std::function<void(const std::string&)> fn) {
        subs_[topic].push_back(std::move(fn));
    }
    void publish(const std::string& topic, const std::string& payload) {
        auto it = subs_.find(topic);
        if (it == subs_.end()) return; // no listeners: nothing happens
        for (const auto& fn : it->second)
            fn(payload);
    }
};

int main() {
    EventBus bus;

    // Independent modules subscribe without referencing each other.
    bus.subscribe("order.placed", [](const std::string& id) {
        std::cout << "  email: confirm order " << id << "\n";
    });
    bus.subscribe("order.placed", [](const std::string& id) {
        std::cout << "  inventory: reserve stock for " << id << "\n";
    });
    bus.subscribe("order.cancelled", [](const std::string& id) {
        std::cout << "  refund: process for " << id << "\n";
    });

    std::cout << "publish order.placed A100\n";
    bus.publish("order.placed", "A100");
    std::cout << "publish order.cancelled A100\n";
    bus.publish("order.cancelled", "A100");
    std::cout << "publish shipping.delayed A100 (no subscribers)\n";
    bus.publish("shipping.delayed", "A100"); // silently no-op
    return 0;
}
