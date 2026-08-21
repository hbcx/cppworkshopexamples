#pragma once
// The logger moved onto a background thread. A log call now formats a record and
// pushes it onto a bounded queue and returns; a single worker thread pops
// records and writes them to the sinks. The slow part (formatting is cheap, but
// the file or socket write is not) leaves the calling thread's hot path.
#include "queue.hpp"

#include <atomic>
#include <chrono>
#include <format>
#include <functional>
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

// Deterministic clock for reproducible demo output. Single-threaded only: its
// state is not synchronised, so use it from ONE producer. The concurrent part of
// the demo uses the default std::chrono::system_clock::now, which is thread-safe.
struct StepClock {
    std::chrono::system_clock::time_point time;
    std::chrono::milliseconds step;
    std::chrono::system_clock::time_point operator()() {
        auto current = time;
        time += step;
        return current;
    }
};

// The async logger. Sinks and the clock are fixed at construction; the sinks are
// touched only by the worker thread, and the clock is called on the logging
// threads (the default one is thread-safe). The queue is the single synchronised
// handoff between them.
class AsyncLogger {
public:
    AsyncLogger(std::size_t capacity, std::vector<std::shared_ptr<LogSink>> sinks,
                LogClock clock = std::chrono::system_clock::now)
        : queue_(capacity), sinks_(std::move(sinks)), now_(std::move(clock)) {
        worker_ = std::thread([this] {
            while (std::optional<LogRecord> record = queue_.pop())
                for (auto& sink : sinks_) sink->write(*record);
            for (auto& sink : sinks_) sink->flush();
        });
    }
    ~AsyncLogger() { shutdown(); }

    // Called by any thread: enqueue and return. This is the hot-path cost.
    void emit(LogRecord record) { queue_.push(std::move(record)); }

    std::chrono::system_clock::time_point now() { return now_(); }
    unsigned long long next_seq() { return seq_.fetch_add(1, std::memory_order_relaxed); }

    // Close the queue so the worker drains what is left, then join it.
    void shutdown() {
        queue_.close();
        if (worker_.joinable()) worker_.join();
    }

    AsyncLogger(const AsyncLogger&) = delete;
    AsyncLogger& operator=(const AsyncLogger&) = delete;

private:
    BoundedBlockingQueue<LogRecord> queue_;
    std::vector<std::shared_ptr<LogSink>> sinks_;
    LogClock now_;
    std::atomic<unsigned long long> seq_{0};
    std::thread worker_; // declared last: constructed after the members it uses
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

// Same front-end as example 02, now feeding the async logger: stamp the time and
// call site, format the message, and hand the record to the queue.
template <Level L, Severity Sv, Component C = Component::General, typename... Args>
void log(AsyncLogger& logger, FormatWithLocation<std::type_identity_t<Args>...> fmt, Args&&... args) {
    if constexpr (static_cast<int>(L) >= static_cast<int>(min_level)) {
        logger.emit(LogRecord{logger.now(), L, Sv, C,
                              std::format(fmt.fmt, std::forward<Args>(args)...),
                              basename(fmt.loc.file_name()), fmt.loc.line(),
                              fmt.loc.function_name(), logger.next_seq()});
    }
}
