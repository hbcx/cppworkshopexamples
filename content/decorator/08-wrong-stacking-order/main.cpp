#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Component: a stage in a byte pipeline.
struct Stage {
    virtual std::string run(const std::string& in) const = 0;
    virtual ~Stage() = default;
};

// Source: hands the raw payload to whatever stage sits directly above it.
class Source : public Stage {
public:
    explicit Source(std::string data) : data_(std::move(data)) {}
    std::string run(const std::string&) const override { return data_; }
private:
    std::string data_;
};

// Base decorator: owns the stage below and reaches it through lower().
class StageDecorator : public Stage {
public:
    explicit StageDecorator(std::unique_ptr<Stage> inner) : inner_(std::move(inner)) {}
protected:
    std::string lower(const std::string& in) const { return inner_->run(in); }
private:
    std::unique_ptr<Stage> inner_;
};

// Run-length compress: collapse each run of equal chars into char + count.
// Shrinks data with repetition; expands data that has none.
class Compress : public StageDecorator {
public:
    using StageDecorator::StageDecorator;
    std::string run(const std::string& in) const override {
        const std::string s = lower(in);
        std::string out;
        for (std::size_t i = 0; i < s.size();) {
            std::size_t j = i;
            while (j < s.size() && s[j] == s[i]) ++j;
            out += s[i];
            out += static_cast<char>('0' + static_cast<int>(j - i));
            i = j;
        }
        return out;
    }
};

// "Encrypt": shift each byte by its position, so the output looks random and no
// longer has the runs that Compress needs to shrink anything.
class Encrypt : public StageDecorator {
public:
    using StageDecorator::StageDecorator;
    std::string run(const std::string& in) const override {
        std::string s = lower(in);
        for (std::size_t i = 0; i < s.size(); ++i) {
            const int shifted = (static_cast<unsigned char>(s[i]) + static_cast<int>(i) + 1) % 256;
            s[i] = static_cast<char>(shifted);
        }
        return s;
    }
};

int main() {
    const std::string payload = "aaaaaaaabbbbcccc";   // 16 bytes, highly repetitive

    // Correct: compress INNERMOST, encrypt outermost -- shrink first, then hide.
    std::unique_ptr<Stage> right =
        std::make_unique<Encrypt>(std::make_unique<Compress>(std::make_unique<Source>(payload)));

    // Wrong: encrypt innermost -- it destroys the runs, so Compress can shrink
    // nothing and actually EXPANDS the data. Same layers, silently worse result.
    std::unique_ptr<Stage> wrong =
        std::make_unique<Compress>(std::make_unique<Encrypt>(std::make_unique<Source>(payload)));

    std::cout << "payload:               " << payload.size() << " bytes\n";
    std::cout << "compress then encrypt: " << right->run("").size() << " bytes\n";
    std::cout << "encrypt then compress: " << wrong->run("").size() << " bytes\n";
    return 0;
}
