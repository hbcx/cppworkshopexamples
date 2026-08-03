#include <functional>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

// ANTI-PATTERN: notify() calls observers in a bare loop, so if one throws, the
// exception propagates out and the observers after it are never notified -- a
// half-delivered broadcast. One misbehaving observer silences all the ones behind
// it, and the subject can be left in a half-notified state.
class SubjectBad {
public:
    void subscribe(std::function<void()> h) { handlers_.push_back(std::move(h)); }
    void notify() {
        for (auto& h : handlers_) h();   // a throw here abandons the rest
    }
private:
    std::vector<std::function<void()>> handlers_;
};

// FIX: isolate each observer -- catch around each call, so one observer's failure
// is contained and every other observer is still notified. Report the failures
// rather than swallowing them silently.
class SubjectGood {
public:
    void subscribe(std::function<void()> h) { handlers_.push_back(std::move(h)); }
    void notify() {
        for (auto& h : handlers_) {
            try {
                h();
            } catch (const std::exception& e) {
                std::cout << "observer failed: " << e.what() << " (others still notified)\n";
            }
        }
    }
private:
    std::vector<std::function<void()>> handlers_;
};

int main() {
    std::cout << "== one throw abandons the rest ==\n";
    SubjectBad bad;
    bad.subscribe([] { std::cout << "bad A\n"; });
    bad.subscribe([] { throw std::runtime_error("boom"); });
    bad.subscribe([] { std::cout << "bad C (never reached)\n"; });
    try {
        bad.notify();
    } catch (const std::exception& e) {
        std::cout << "notify aborted by: " << e.what() << "\n";
    }

    std::cout << "== failures isolated per observer ==\n";
    SubjectGood good;
    good.subscribe([] { std::cout << "good A\n"; });
    good.subscribe([] { throw std::runtime_error("boom"); });
    good.subscribe([] { std::cout << "good C (still reached)\n"; });
    good.notify();
    return 0;
}
