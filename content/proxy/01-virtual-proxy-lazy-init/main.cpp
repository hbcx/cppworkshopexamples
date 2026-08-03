#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Subject: the interface the client uses. The proxy and the real object share it,
// so the client cannot tell them apart.
struct Image {
    virtual void display() = 0;
    virtual ~Image() = default;
};

// Real subject: expensive to create -- the load happens in the constructor.
class RealImage : public Image {
public:
    explicit RealImage(std::string file) : file_(std::move(file)) {
        std::cout << "RealImage: loading " << file_ << " from disk\n";
    }
    void display() override { std::cout << "RealImage: drawing " << file_ << "\n"; }
private:
    std::string file_;
};

// Virtual proxy: the same interface, but it does NOT build the RealImage until
// the first display(). Constructing the proxy is free; the cost is deferred to
// first use and paid exactly once.
class ImageProxy : public Image {
public:
    explicit ImageProxy(std::string file) : file_(std::move(file)) {}
    void display() override {
        if (!real_) {
            real_ = std::make_unique<RealImage>(file_);   // load on first use
        }
        real_->display();
    }
private:
    std::string file_;
    std::unique_ptr<RealImage> real_;
};

int main() {
    std::cout << "creating proxy (no load yet)\n";
    ImageProxy img("photo.png");

    std::cout << "first display:\n";
    img.display();     // loads, then draws

    std::cout << "second display:\n";
    img.display();     // already loaded -> just draws
    return 0;
}
