#include <functional>
#include <iostream>
#include <string>
#include <utility>

// A command is essentially a callable, so in modern C++ it is often just a
// std::function: no interface, no class per action. The invoker stores the
// function and triggers it, and a lambda that binds a receiver and its arguments
// is a complete command.
class Button {
public:
    Button(std::string label, std::function<void()> action)
        : label_(std::move(label)), action_(std::move(action)) {}
    void press() const {
        std::cout << "[" << label_ << "] ";
        action_();
    }
private:
    std::string label_;
    std::function<void()> action_;
};

// Receiver.
class Document {
public:
    void save() { std::cout << "document saved\n"; }
    void print(int copies) { std::cout << "printing " << copies << " copies\n"; }
};

int main() {
    Document doc;

    Button save("Save", [&doc] { doc.save(); });
    Button print3("Print", [&doc] { doc.print(3); });   // arguments bound inside the lambda

    save.press();     // [Save] document saved
    print3.press();   // [Print] printing 3 copies
    return 0;
}
