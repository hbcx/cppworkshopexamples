#include <iostream>
#include <string>

// Anti-pattern: the pre-C++11 lazy singleton -- a static raw pointer, a null
// check, and new on first use.
class LegacyLog {
public:
    static LegacyLog& instance() {
        // DATA RACE (described, NOT executed): under threads two callers can both
        // see p_ == nullptr and both run new, and the pointer write can be seen
        // before the constructor's writes. The old fix was a mutex / double-checked
        // locking -- machinery C++11 makes unnecessary. Shown single-threaded here.
        if (!p_) p_ = new LegacyLog();
        return *p_;   // and nothing ever deletes p_ -> the destructor never runs
    }
    void log(const std::string& m) { std::cout << "[legacy] " << m << '\n'; }
    ~LegacyLog() { std::cout << "[legacy] destructor (flush/close)\n"; }

    LegacyLog(const LegacyLog&) = delete;
    LegacyLog& operator=(const LegacyLog&) = delete;

private:
    LegacyLog() { std::cout << "[legacy] constructed\n"; }
    static LegacyLog* p_;
};
LegacyLog* LegacyLog::p_ = nullptr;

// Fix: the Meyers Singleton. Lazy, thread-safe since C++11, and destroyed at
// program exit -- so the destructor actually runs. No pointer, no mutex.
class ModernLog {
public:
    static ModernLog& instance() {
        static ModernLog inst;
        return inst;
    }
    void log(const std::string& m) { std::cout << "[modern] " << m << '\n'; }
    ~ModernLog() { std::cout << "[modern] destructor (flush/close)\n"; }

    ModernLog(const ModernLog&) = delete;
    ModernLog& operator=(const ModernLog&) = delete;

private:
    ModernLog() { std::cout << "[modern] constructed\n"; }
};

int main() {
    LegacyLog::instance().log("writing a line");
    ModernLog::instance().log("writing a line");
    // At exit only "[modern] destructor" prints: the raw-pointer singleton leaked,
    // so its flush/close never happened.
    return 0;
}
