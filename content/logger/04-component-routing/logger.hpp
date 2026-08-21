#pragma once
// The full logger: async (a background worker), with routing. One record can go
// to several destinations at once -- a set of global sinks that see everything,
// a per-component sink so a subsystem gets its own file, and a critical sink that
// only receives the highest-severity records (an alerting channel).
//
// It also adds the two ergonomics real code wants: an application-installed
// global logger so components do not pass the object around, and per-component
// front-ends (log_storage, log_network) that bind the component once.
#include "queue.hpp"

#include <atomic>
#include <chrono>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <source_location>
#include <string>
#include <thread>
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

using SinkList = std::vector<std::shared_ptr<LogSink>>;

// The async, routing logger. The routing config and the clock are fixed at
// construction; the config is read only by the worker thread, so it needs no
// lock, and the queue is the single synchronised handoff from the logging
// threads to the worker.
class RoutingLogger {
public:
    struct Config {
        SinkList global;                          // every record
        std::map<Component, SinkList> byComponent; // records of that component
        SinkList critical;                        // records with severity == Critical
    };

    RoutingLogger(std::size_t capacity, Config config,
                  LogClock clock = std::chrono::system_clock::now)
        : queue_(capacity), config_(std::move(config)), now_(std::move(clock)) {
        worker_ = std::thread([this] {
            while (std::optional<LogRecord> record = queue_.pop())
                dispatch(*record);
            flush_all();
        });
    }
    ~RoutingLogger() { shutdown(); }

    void emit(LogRecord record) { queue_.push(std::move(record)); }
    std::chrono::system_clock::time_point now() { return now_(); }
    unsigned long long next_seq() { return seq_.fetch_add(1, std::memory_order_relaxed); }

    void shutdown() {
        queue_.close();
        if (worker_.joinable()) worker_.join();
    }

    RoutingLogger(const RoutingLogger&) = delete;
    RoutingLogger& operator=(const RoutingLogger&) = delete;

private:
    // Fan one record out to every destination that wants it. A single record can
    // land in the global sinks AND its component's own sink AND the critical sink.
    void dispatch(const LogRecord& record) {
        for (auto& sink : config_.global) sink->write(record);
        if (auto it = config_.byComponent.find(record.component); it != config_.byComponent.end())
            for (auto& sink : it->second) sink->write(record);
        if (record.severity == Severity::Critical)
            for (auto& sink : config_.critical) sink->write(record);
    }
    void flush_all() {
        for (auto& sink : config_.global) sink->flush();
        for (auto& [component, sinks] : config_.byComponent)
            for (auto& sink : sinks) sink->flush();
        for (auto& sink : config_.critical) sink->flush();
    }

    BoundedBlockingQueue<LogRecord> queue_;
    Config config_;
    LogClock now_;
    std::atomic<unsigned long long> seq_{0};
    std::thread worker_;
};

template <typename... Args>
struct FormatWithLocation {
    std::format_string<Args...> fmt;
    std::source_location loc;
    template <typename S>
    consteval FormatWithLocation(const S& s,
                                 std::source_location l = std::source_location::current())
        : fmt(s), loc(l) {}
};

constexpr Level min_level = Level::Debug;

// The generic front-end, taking the logger explicitly. Still available for any
// case that does not fit the per-component wrappers below.
template <Level L, Severity Sv, Component C = Component::General, typename... Args>
void log(RoutingLogger& logger, FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) {
    if constexpr (static_cast<int>(L) >= static_cast<int>(min_level)) {
        logger.emit(LogRecord{logger.now(), L, Sv, C,
                              std::format(fmt.fmt, std::forward<Args>(args)...),
                              basename(fmt.loc.file_name()), fmt.loc.line(),
                              fmt.loc.function_name(), logger.next_seq()});
    }
}

// --- the global logger ------------------------------------------------------
// The application creates one RoutingLogger and installs it here, so components
// log without being handed the object. The owner (main) controls its lifetime,
// which avoids the static-destruction ordering trap a Meyers singleton -- whose
// worker thread might outlive objects it uses -- would bring. It is a plain
// pointer set once at startup and read afterwards, so no synchronisation.
inline RoutingLogger*& global_logger_slot() {
    static RoutingLogger* instance = nullptr;
    return instance;
}
inline void set_global_logger(RoutingLogger& logger) { global_logger_slot() = &logger; }
inline RoutingLogger& global_logger() { return *global_logger_slot(); }

// --- per-component front-ends -----------------------------------------------
// Bind the component once so a subsystem calls log_storage(...) instead of
// log<..., Component::Storage>(global_logger(), ...). The generic log above
// stays available. The call site is preserved: the FormatWithLocation captures
// the location where the argument is built (the caller), and forwarding it
// through this wrapper does not move it.
template <Level L, Severity Sv, typename... Args>
void log_storage(FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) {
    log<L, Sv, Component::Storage>(global_logger(), fmt, std::forward<Args>(args)...);
}
template <Level L, Severity Sv, typename... Args>
void log_network(FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) {
    log<L, Sv, Component::Network>(global_logger(), fmt, std::forward<Args>(args)...);
}
