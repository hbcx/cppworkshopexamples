#include <iostream>

// Receiver: the object that knows how to do the actual work.
class Light {
public:
    void on() { std::cout << "light: on\n"; }
    void off() { std::cout << "light: off\n"; }
};

// Command: a request turned into an object with one method.
struct Command {
    virtual void execute() = 0;
    virtual ~Command() = default;
};

// Concrete commands each bind a receiver (and could bind arguments) and forward
// execute() to it.
class LightOn : public Command {
public:
    explicit LightOn(Light& l) : light_(l) {}
    void execute() override { light_.on(); }
private:
    Light& light_;
};

class LightOff : public Command {
public:
    explicit LightOff(Light& l) : light_(l) {}
    void execute() override { light_.off(); }
private:
    Light& light_;
};

// Invoker: triggers a command without knowing what it does or which object it
// acts on. It depends only on the Command interface, so the same button drives
// any command.
class Button {
public:
    void setCommand(Command* c) { command_ = c; }
    void press() { if (command_) command_->execute(); }
private:
    Command* command_ = nullptr;
};

int main() {
    Light light;
    LightOn on(light);
    LightOff off(light);

    Button button;
    button.setCommand(&on);
    button.press();          // light: on

    button.setCommand(&off);
    button.press();          // light: off
    return 0;
}
