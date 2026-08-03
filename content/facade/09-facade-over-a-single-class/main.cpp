#include <iostream>
#include <string>

// A single, already-simple class.
class FileLogger {
public:
    void log(const std::string& msg) { std::cout << "log: " << msg << "\n"; }
};

// ANTI-PATTERN: a facade over ONE class that forwards one-to-one. It hides no
// complexity, coordinates nothing, and adds a layer, an indirection, and a file
// to maintain for zero simplification. A facade earns its place by unifying
// SEVERAL classes behind one call; over one it is just a pass-through.
class LoggerFacade {
public:
    void log(const std::string& msg) { logger_.log(msg); }   // 1:1, adds nothing
private:
    FileLogger logger_;
};

// FIX: use the class directly. A real facade appears only once there are several
// classes to coordinate behind one call, as in the basics example.
int main() {
    std::cout << "== needless facade ==\n";
    LoggerFacade facade;
    facade.log("started");

    std::cout << "== the class, used directly ==\n";
    FileLogger logger;
    logger.log("started");
    return 0;
}
