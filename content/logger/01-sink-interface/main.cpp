#include "log.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

// A custom sink -- the reason LogSink is an interface. This one stands in for a
// network target: instead of a file it appends each formatted line to a buffer
// that represents the bytes it would put on the wire. In real code the marked
// line is where you would send() to a socket.
class NetworkSink : public LogSink {
public:
    void write(const LogRecord& record) override {
        sent_ += format_line(record);
        sent_ += '\n'; // <-- a real sink would send these bytes to a socket here
    }
    const std::string& sent() const { return sent_; }

private:
    std::string sent_;
};

int main() {
    // One logger, two destinations at once: a "file" (an in-memory stream, so
    // the demo stays self-contained) and the custom network sink above.
    std::ostringstream file;
    auto fileSink = std::make_shared<StreamSink>(file);
    auto netSink = std::make_shared<NetworkSink>();

    // A deterministic clock so the timestamps in the output are reproducible;
    // real code would leave this out and get std::chrono::system_clock::now.
    using namespace std::chrono;
    auto base = sys_days{year{2026} / 8 / 21} + 9h + 30min;
    Logger logger{StepClock{base, 250ms}};
    logger.add_sink(fileSink);
    logger.add_sink(netSink);

    logger.log(Level::Info, Severity::Minor, Component::General, "service starting");
    logger.log(Level::Error, Severity::Critical, Component::Network, "connection refused");
    logger.flush();

    // Both sinks received the same two records, in order.
    std::cout << "--- file sink ---\n" << file.str();
    std::cout << "--- network sink (bytes it would send) ---\n" << netSink->sent();
    return 0;
}
