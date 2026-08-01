#include <iostream>
#include <string>

struct Logger {
    virtual void log(const std::string& level, const std::string& msg) = 0;
    virtual ~Logger() = default;
};

class LegacyLog {
public:
    void write(const std::string& line) {
        std::cout << "LEGACY| " << line << '\n';
    }
};

// Class adapter: inherit the Target publicly (it IS a Logger) and the Adaptee
// PRIVATELY (implemented in terms of LegacyLog, but not a LegacyLog to the
// outside). log() calls the inherited write() directly -- no wrapped member.
class LegacyLogAdapter : public Logger, private LegacyLog {
public:
    void log(const std::string& level, const std::string& msg) override {
        write("[" + level + "] " + msg);   // inherited from LegacyLog
    }
};

void runApp(Logger& logger) {
    logger.log("INFO", "app started");
    logger.log("ERROR", "out of memory");
}

int main() {
    LegacyLogAdapter adapter;
    runApp(adapter);

    // Private inheritance keeps the adaptee interface hidden: to the outside the
    // adapter is only a Logger, not a LegacyLog, so the next line would NOT
    // compile -- the translation cannot be bypassed.
    // adapter.write("bypass");
    return 0;
}
