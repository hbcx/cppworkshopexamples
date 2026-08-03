#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// The subject stores callbacks, not observer objects. A subscriber is any
// callable with the right signature, so a lambda registers directly -- no
// Observer interface and no class per subscriber. This is the modern C++ form,
// and the shape of a signal / event.
class Button {
public:
    void onClick(std::function<void(const std::string&)> handler) {
        handlers_.push_back(std::move(handler));
    }
    void click(const std::string& label) {
        for (const auto& h : handlers_) h(label);   // notify every subscriber
    }
private:
    std::vector<std::function<void(const std::string&)>> handlers_;
};

int main() {
    Button save;

    int clicks = 0;
    save.onClick([](const std::string& l) { std::cout << "log: clicked " << l << "\n"; });
    save.onClick([&clicks](const std::string&) { ++clicks; });   // a lambda that captures state

    save.click("Save");
    save.click("Save");
    std::cout << "clicks counted: " << clicks << "\n";
    return 0;
}
