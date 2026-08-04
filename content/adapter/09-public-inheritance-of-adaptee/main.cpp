#include <iostream>
#include <string>

struct Logger {
    virtual void log(const std::string& msg) = 0;
    virtual ~Logger() = default;
};

class LegacyLog {
public:
    void write(const std::string& line) { std::cout << "LEGACY| " << line << '\n'; }
    void setRaw(bool on) { raw_ = on; }   // adaptee detail that should stay hidden
private:
    bool raw_ = false;
};

// Anti-pattern: a class adapter that inherits the adaptee PUBLICLY. The adapter
// now IS-A LegacyLog to the whole world: every LegacyLog method leaks through
// (write, setRaw), the translation can be bypassed, and callers can come to
// depend on the adaptee's interface -- defeating the point of adapting.
class LeakyAdapter : public Logger, public LegacyLog {
public:
    void log(const std::string& msg) override { write("[log] " + msg); }
};

// Fix: inherit the adaptee PRIVATELY (implemented-in-terms-of). The adapter is
// only a Logger to the outside; the adaptee stays encapsulated and the
// translation cannot be avoided. (Composition -- a LegacyLog member -- is
// the other, usually preferable, fix.)
class TightAdapter : public Logger, private LegacyLog {
public:
    void log(const std::string& msg) override { write("[log] " + msg); }
};

int main() {
    LeakyAdapter leaky;
    leaky.log("hello");
    // The leak: adaptee methods are reachable through the adapter, bypassing it.
    leaky.write("bypassed the adapter entirely");
    leaky.setRaw(true);
    std::cout << "leaky exposes adaptee: write() and setRaw() callable directly\n";

    TightAdapter tight;
    tight.log("hello");
    // tight.write("...");   // would NOT compile: adaptee interface is hidden
    std::cout << "tight hides adaptee: only log() is callable\n";
    return 0;
}
