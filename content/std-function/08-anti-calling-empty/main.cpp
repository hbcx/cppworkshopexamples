#include <functional>
#include <iostream>

// TRAP: an optional callback stored as a std::function, then called without
// checking whether it was ever set. An unset std::function is empty, and
// calling it throws std::bad_function_call -- so the "optional" callback turns
// into an exception on the first use.
class Downloader {
public:
    using ProgressFn = std::function<void(int)>;
    void setProgress(ProgressFn f) { progress_ = std::move(f); }

    void run() {
        for (int pct : {25, 50, 100})
            notifyBad(pct);          // throws if no callback was set
    }
    void runSafe() {
        for (int pct : {25, 50, 100})
            notifyGood(pct);         // guarded, does nothing if unset
    }
private:
    void notifyBad(int pct)  { progress_(pct); }              // WRONG: no guard
    void notifyGood(int pct) { if (progress_) progress_(pct); } // FIX: guard first
    ProgressFn progress_;
};

int main() {
    Downloader d;   // note: no progress callback set

    try {
        d.run();
    } catch (const std::bad_function_call&) {
        std::cout << "unguarded call threw bad_function_call\n";
    }

    std::cout << "guarded run: ";
    d.runSafe();                     // silently skips the missing callback
    std::cout << "completed with no callback set\n";
    return 0;
}
