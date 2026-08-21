#pragma once
// The core logging model: a record, an output-sink interface, and a logger that
// fans each record out to every sink. This file has no threading yet (that is
// example 03) and no fancy call API yet (that is example 02) -- it isolates the
// one idea that everything else builds on: WHERE a log goes is a pluggable sink.
#include <chrono>
#include <format>
#include <functional>
#include <memory>
#include <ostream>
#include <source_location>
#include <string>
#include <vector>

enum class Level { Debug, Info, Error };
enum class Severity { Minor, Major, Critical };
enum class Component { General, Network, Storage, Auth };

inline const char* to_string(Level l) {
    switch (l) {
        case Level::Debug: return "DEBUG";
        case Level::Info:  return "INFO";
        case Level::Error: return "ERROR";
    }
    return "?";
}
inline const char* to_string(Severity s) {
    switch (s) {
        case Severity::Minor:    return "minor";
        case Severity::Major:    return "major";
        case Severity::Critical: return "CRITICAL";
    }
    return "?";
}
inline const char* to_string(Component c) {
    switch (c) {
        case Component::General: return "general";
        case Component::Network: return "network";
        case Component::Storage: return "storage";
        case Component::Auth:    return "auth";
    }
    return "?";
}

// One log entry. file and function point into static strings owned by
// source_location, so keeping the pointers (even across threads later) is safe.
struct LogRecord {
    std::chrono::system_clock::time_point time; // when the event happened
    Level level;
    Severity severity;
    Component component;
    std::string message;
    const char* file;        // basename of the source file
    unsigned line;
    const char* function;
    unsigned long long seq;  // monotonic order within one logger
};

// The compilers spell a parameter list differently (const char* vs const
// char *), so keep only the part of function_name before '(' -- the return type
// and qualified name, which they agree on.
inline std::string func_head(const char* full) {
    const char* p = full;
    while (*p && *p != '(') ++p;
    return std::string(full, p);
}

// Render a record to one line, timestamp first. The time is floored to
// milliseconds so the fractional part stays short and readable.
inline std::string format_line(const LogRecord& r) {
    auto when = std::chrono::floor<std::chrono::milliseconds>(r.time);
    return std::format("{:%Y-%m-%d %H:%M:%S} #{} [{}|{}|{}] {} ({}:{} {})", when,
                       r.seq, to_string(r.level), to_string(r.severity),
                       to_string(r.component), r.message, r.file, r.line,
                       func_head(r.function));
}

// THE extension point. Implement this interface and you have a new destination
// -- a file, a socket, a database, an in-memory buffer for tests -- with no
// change to any calling code.
struct LogSink {
    virtual ~LogSink() = default;
    virtual void write(const LogRecord& record) = 0;
    virtual void flush() {}
};

// A ready-made sink that writes formatted lines to any std::ostream: a file via
// std::ofstream, std::cout, or a std::ostringstream (as the demo uses).
class StreamSink : public LogSink {
public:
    explicit StreamSink(std::ostream& out) : out_(out) {}
    void write(const LogRecord& record) override { out_ << format_line(record) << '\n'; }
    void flush() override { out_.flush(); }

private:
    std::ostream& out_;
};

inline const char* basename(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\') b = p + 1;
    return b;
}

// The clock is a dependency, defaulting to the real wall clock. Injecting a
// different one is how the demos get reproducible timestamps and how you would
// test time-dependent logging.
using LogClock = std::function<std::chrono::system_clock::time_point()>;

// A deterministic clock for the demos and for tests: starts at a fixed instant
// and advances a fixed step on each call, so the timestamps in the output are
// reproducible. Single-threaded use only (its state is not synchronised).
struct StepClock {
    std::chrono::system_clock::time_point time;
    std::chrono::milliseconds step;
    std::chrono::system_clock::time_point operator()() {
        auto current = time;
        time += step;
        return current;
    }
};

// A synchronous logger: build a record (capturing the call site and the time)
// and hand it to every sink, in order. Level, severity and component are plain
// arguments here; example 02 promotes them to compile-time template parameters.
class Logger {
public:
    explicit Logger(LogClock clock = std::chrono::system_clock::now) : now_(std::move(clock)) {}

    void add_sink(std::shared_ptr<LogSink> sink) { sinks_.push_back(std::move(sink)); }

    void log(Level level, Severity severity, Component component, std::string message,
             std::source_location loc = std::source_location::current()) {
        LogRecord record{now_(),
                         level,
                         severity,
                         component,
                         std::move(message),
                         basename(loc.file_name()),
                         loc.line(),
                         loc.function_name(),
                         seq_++};
        for (auto& sink : sinks_) sink->write(record);
    }

    void flush() {
        for (auto& sink : sinks_) sink->flush();
    }

private:
    std::vector<std::shared_ptr<LogSink>> sinks_;
    unsigned long long seq_ = 0;
    LogClock now_;
};
