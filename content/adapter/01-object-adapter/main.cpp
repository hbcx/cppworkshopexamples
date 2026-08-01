#include <iostream>
#include <string>

// Target: the interface our code expects. Client code talks only to this.
struct Logger {
    virtual void log(const std::string& level, const std::string& msg) = 0;
    virtual ~Logger() = default;
};

// Adaptee: an existing class with a useful implementation but the wrong shape --
// one write(line), no notion of a level. We cannot, or do not want to, change it.
class LegacyLog {
public:
    void write(const std::string& line) {
        std::cout << "LEGACY| " << line << '\n';
    }
};

// Adapter: implements the Target by HOLDING an Adaptee and translating each
// call. This is the object adapter -- it wraps an instance (composition).
class LegacyLogAdapter : public Logger {
public:
    explicit LegacyLogAdapter(LegacyLog& legacy) : legacy_(legacy) {}
    void log(const std::string& level, const std::string& msg) override {
        legacy_.write("[" + level + "] " + msg);   // translate to write(line)
    }
private:
    LegacyLog& legacy_;
};

// Client: knows only the Target interface, nothing about LegacyLog.
void runApp(Logger& logger) {
    logger.log("INFO", "app started");
    logger.log("WARN", "disk almost full");
}

int main() {
    LegacyLog legacy;
    LegacyLogAdapter adapter(legacy);
    runApp(adapter);   // the legacy class, used through the modern interface
    return 0;
}
