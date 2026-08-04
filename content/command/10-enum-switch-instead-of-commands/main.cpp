#include <iostream>

class Light {
public:
    void on() { std::cout << "on\n"; }
    void off() { std::cout << "off\n"; }
    void dim() { std::cout << "dim\n"; }
};

// ANTI-PATTERN: the invoker switches on an action enum. Every new action edits
// this switch (and any queue, log, or undo built on the enum), the invoker is
// coupled to every action and its receiver, and you cannot store a CONFIGURED
// action -- an action plus its arguments -- as a single value the way a command
// object lets you.
enum class Action { On, Off, Dim };

class RemoteBad {
public:
    explicit RemoteBad(Light& l) : light_(l) {}
    void trigger(Action a) {
        switch (a) {
            case Action::On:  light_.on();  break;
            case Action::Off: light_.off(); break;
            case Action::Dim: light_.dim(); break;
        }
    }
private:
    Light& light_;
};

// FIX: each action is a command object. The invoker holds a command and triggers
// it, knowing none of them; a new action is a new command, and a command can be
// stored, queued, logged, or undone -- none of which an enum value can.
struct Command {
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class On : public Command {
public:
    explicit On(Light& l) : light_(l) {}
    void execute() override { light_.on(); }
private:
    Light& light_;
};

class Off : public Command {
public:
    explicit Off(Light& l) : light_(l) {}
    void execute() override { light_.off(); }
private:
    Light& light_;
};

class RemoteGood {
public:
    void press(Command& c) { c.execute(); }
};

int main() {
    Light light;

    std::cout << "== enum + switch ==\n";
    RemoteBad bad(light);
    bad.trigger(Action::On);
    bad.trigger(Action::Dim);

    std::cout << "== command objects ==\n";
    On on(light);
    Off off(light);
    RemoteGood good;
    good.press(on);
    good.press(off);
    return 0;
}
