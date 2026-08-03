#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Component: a sink that records the lines written to it.
struct Sink {
    virtual void write(const std::string& line) = 0;
    virtual ~Sink() = default;
};

class MemorySink : public Sink {
public:
    void write(const std::string& line) override { lines_.push_back(line); }
    int count() const { return static_cast<int>(lines_.size()); }
private:
    std::vector<std::string> lines_;
};

// ANTI-PATTERN: this decorator adds its own behaviour (a log line) but FORGETS
// to forward to the wrapped sink, so the real write never happens. It compiles,
// it appears to work -- there is even output -- yet the data is silently dropped.
class LoudSinkBad : public Sink {
public:
    explicit LoudSinkBad(std::unique_ptr<Sink> inner) : inner_(std::move(inner)) {}
    void write(const std::string& line) override {
        std::cout << "log: writing " << line << '\n';
        // BUG: missing  inner_->write(line);  -- the wrapped sink never sees it.
    }
private:
    std::unique_ptr<Sink> inner_;
};

// FIX: a decorator must delegate to the wrapped component -- add behaviour
// AROUND the call, never INSTEAD of it.
class LoudSinkGood : public Sink {
public:
    explicit LoudSinkGood(std::unique_ptr<Sink> inner) : inner_(std::move(inner)) {}
    void write(const std::string& line) override {
        std::cout << "log: writing " << line << '\n';
        inner_->write(line);        // forward -- the whole point of a decorator
    }
private:
    std::unique_ptr<Sink> inner_;
};

int main() {
    auto base1 = std::make_unique<MemorySink>();
    MemorySink* raw1 = base1.get();
    LoudSinkBad bad(std::move(base1));
    bad.write("a");
    bad.write("b");
    std::cout << "stored (broken): " << raw1->count() << '\n';   // 0 -- dropped

    auto base2 = std::make_unique<MemorySink>();
    MemorySink* raw2 = base2.get();
    LoudSinkGood good(std::move(base2));
    good.write("a");
    good.write("b");
    std::cout << "stored (fixed): " << raw2->count() << '\n';     // 2
    return 0;
}
