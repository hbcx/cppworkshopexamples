#include <iostream>
#include <string>

// Implementor: a device the remote controls. Low-level operations only; a device
// knows nothing about remotes.
struct Device {
    virtual void setPower(bool on) = 0;
    virtual void setVolume(int v) = 0;
    virtual std::string name() const = 0;
    virtual ~Device() = default;
};

class TV : public Device {
public:
    void setPower(bool on) override { std::cout << "TV power " << (on ? "on" : "off") << "\n"; }
    void setVolume(int v) override { std::cout << "TV volume " << v << "\n"; }
    std::string name() const override { return "TV"; }
};

class Radio : public Device {
public:
    void setPower(bool on) override { std::cout << "Radio power " << (on ? "on" : "off") << "\n"; }
    void setVolume(int v) override { std::cout << "Radio volume " << v << "\n"; }
    std::string name() const override { return "Radio"; }
};

// Abstraction: a remote built ON TOP of a Device. It holds the implementor and
// offers higher-level operations in terms of the device's primitives.
class RemoteControl {
public:
    explicit RemoteControl(Device& d) : device_(d) {}
    virtual ~RemoteControl() = default;
    void togglePower() {
        on_ = !on_;
        device_.setPower(on_);
    }
protected:
    Device& device_;
    bool on_ = false;
};

// Refined abstraction: extends the abstraction side with more behaviour, still
// over ANY Device. The abstraction hierarchy grows on its own axis -- adding this
// remote touches no device, and it works with every device that exists.
class AdvancedRemote : public RemoteControl {
public:
    using RemoteControl::RemoteControl;
    void mute() {
        std::cout << "muting " << device_.name() << "\n";
        device_.setVolume(0);
    }
};

int main() {
    TV tv;
    Radio radio;

    RemoteControl basic(tv);
    basic.togglePower();          // TV power on

    AdvancedRemote advanced(radio);   // the refined remote, over a different device
    advanced.togglePower();       // Radio power on
    advanced.mute();              // muting Radio / Radio volume 0
    return 0;
}
