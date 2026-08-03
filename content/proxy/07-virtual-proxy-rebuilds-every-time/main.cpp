#include <iostream>
#include <memory>
#include <string>
#include <utility>

struct Image {
    virtual void display() = 0;
    virtual ~Image() = default;
};

class RealImage : public Image {
public:
    explicit RealImage(std::string f) : file_(std::move(f)) {
        std::cout << "loading " << file_ << "\n";      // the expensive step
    }
    void display() override { std::cout << "drawing " << file_ << "\n"; }
private:
    std::string file_;
};

// ANTI-PATTERN: a virtual proxy that creates the real object on EVERY call
// because it never stores it. The whole point of a virtual proxy is to build
// once, on first use; rebuilding each time is worse than eager construction --
// the client pays the load cost again and again.
class RebuildingProxy : public Image {
public:
    explicit RebuildingProxy(std::string f) : file_(std::move(f)) {}
    void display() override {
        RealImage real(file_);          // built and destroyed every call -- BUG
        real.display();
    }
private:
    std::string file_;
};

// FIX: build once and keep it -- create on first use, reuse thereafter.
class CachingProxy : public Image {
public:
    explicit CachingProxy(std::string f) : file_(std::move(f)) {}
    void display() override {
        if (!real_) real_ = std::make_unique<RealImage>(file_);
        real_->display();
    }
private:
    std::string file_;
    std::unique_ptr<RealImage> real_;
};

int main() {
    std::cout << "== rebuilding proxy ==\n";
    RebuildingProxy bad("a.png");
    bad.display();
    bad.display();
    bad.display();     // three loads

    std::cout << "== caching proxy ==\n";
    CachingProxy good("b.png");
    good.display();
    good.display();
    good.display();    // one load
    return 0;
}
