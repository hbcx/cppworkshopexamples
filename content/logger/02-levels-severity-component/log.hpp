#pragma once
// The logger from example 01, given an ergonomic front-end: a single function
// template log<Level, Severity, Component>(...) whose level, severity and
// component are compile-time template parameters, whose message uses std::format
// with compile-time-checked arguments, and whose call site is captured
// automatically. A level below the compile-time minimum is removed by the
// compiler.
#include <chrono>
#include <format>
#include <functional>
#include <memory>
#include <ostream>
#include <source_location>
#include <string>
#include <type_traits>
#include <utility>
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

struct LogRecord {
    std::chrono::system_clock::time_point time;
    Level level;
    Severity severity;
    Component component;
    std::string message;
    const char* file;
    unsigned line;
    const char* function;
    unsigned long long seq;
};

// The compilers spell a parameter list differently (const char* vs const
// char *), so keep only the part of function_name before '(' -- the return type
// and qualified name, which they agree on.
inline std::string func_head(const char* full) {
    const char* p = full;
    while (*p && *p != '(') ++p;
    return std::string(full, p);
}

inline std::string format_line(const LogRecord& r) {
    auto when = std::chrono::floor<std::chrono::milliseconds>(r.time);
    return std::format("{:%Y-%m-%d %H:%M:%S} #{} [{}|{}|{}] {} ({}:{} {})", when,
                       r.seq, to_string(r.level), to_string(r.severity),
                       to_string(r.component), r.message, r.file, r.line,
                       func_head(r.function));
}

struct LogSink {
    virtual ~LogSink() = default;
    virtual void write(const LogRecord& record) = 0;
    virtual void flush() {}
};

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

using LogClock = std::function<std::chrono::system_clock::time_point()>;

// Deterministic clock for reproducible demo output (fixed start, fixed step).
// Real code uses the default std::chrono::system_clock::now. Single-threaded.
struct StepClock {
    std::chrono::system_clock::time_point time;
    std::chrono::milliseconds step;
    std::chrono::system_clock::time_point operator()() {
        auto current = time;
        time += step;
        return current;
    }
};

class Logger {
public:
    explicit Logger(LogClock clock = std::chrono::system_clock::now) : now_(std::move(clock)) {}

    void add_sink(std::shared_ptr<LogSink> sink) { sinks_.push_back(std::move(sink)); }
    void emit(LogRecord record) {
        for (auto& sink : sinks_) sink->write(record);
    }
    std::chrono::system_clock::time_point now() { return now_(); }
    unsigned long long next_seq() { return seq_++; }
    void flush() {
        for (auto& sink : sinks_) sink->flush();
    }

private:
    std::vector<std::shared_ptr<LogSink>> sinks_;
    unsigned long long seq_ = 0;
    LogClock now_;
};

// A format string bundled with its call-site location. The consteval constructor
// captures source_location::current() as a default argument -- the trick that
// lets a variadic function still know where it was called from.
template <typename... Args>
struct FormatWithLocation {
    std::format_string<Args...> fmt;
    std::source_location loc;
    template <typename S>
    consteval FormatWithLocation(const S& s,
                                 std::source_location l = std::source_location::current())
        : fmt(s), loc(l) {}
};

// The compile-time minimum level. Any log below this is dropped by the compiler
// (see the if constexpr in log()). Change it per build to turn debug logging on
// or off with zero runtime cost.
constexpr Level min_level = Level::Info;

// The front-end. Level/Severity/Component are compile-time template parameters;
// the message is a checked std::format string with its arguments; the call site
// and the time are captured automatically. std::type_identity_t puts the format
// parameter in a non-deduced context so Args deduce only from the trailing args.
template <Level L, Severity Sv, Component C = Component::General, typename... Args>
void log(Logger& logger, FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) {
    if constexpr (static_cast<int>(L) >= static_cast<int>(min_level)) {
        logger.emit(LogRecord{logger.now(), L, Sv, C,
                              std::format(fmt.fmt, std::forward<Args>(args)...),
                              basename(fmt.loc.file_name()), fmt.loc.line(),
                              fmt.loc.function_name(), logger.next_seq()});
    }
}
