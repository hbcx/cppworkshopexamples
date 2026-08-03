#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

// ANTI-PATTERN: the shared flyweight carries MUTABLE, per-use state (a colour).
// Because the same object backs every use of the character, setting the colour
// for one occurrence changes it for ALL of them -- a flyweight has no per-instance
// identity to hold context, so context stored on it leaks across every user.
class GlyphBad {
public:
    explicit GlyphBad(char c) : c_(c) {}
    void setColor(std::string color) { color_ = std::move(color); }   // mutates shared state -- BUG
    void draw() const { std::cout << c_ << " in " << color_ << "\n"; }
private:
    char c_;
    std::string color_ = "black";
};

class GlyphFactoryBad {
public:
    std::shared_ptr<GlyphBad> get(char c) {
        auto& g = pool_[c];
        if (!g) g = std::make_shared<GlyphBad>(c);
        return g;
    }
private:
    std::map<char, std::shared_ptr<GlyphBad>> pool_;
};

// FIX: the flyweight holds only IMMUTABLE intrinsic state (the character). The
// colour is extrinsic -- supplied per use -- so each occurrence draws in its own
// colour and no use can disturb another.
class GlyphGood {
public:
    explicit GlyphGood(char c) : c_(c) {}
    void drawIn(const std::string& color) const { std::cout << c_ << " in " << color << "\n"; }
private:
    char c_;
};

class GlyphFactoryGood {
public:
    std::shared_ptr<GlyphGood> get(char c) {
        auto& g = pool_[c];
        if (!g) g = std::make_shared<GlyphGood>(c);
        return g;
    }
private:
    std::map<char, std::shared_ptr<GlyphGood>> pool_;
};

int main() {
    std::cout << "== mutable shared state ==\n";
    GlyphFactoryBad bad;
    auto first = bad.get('a');
    first->setColor("red");
    auto second = bad.get('a');       // the SAME shared object
    second->setColor("blue");         // overwrites the red set above
    first->draw();                    // a in blue -- not red!
    second->draw();                   // a in blue

    std::cout << "== colour as extrinsic state ==\n";
    GlyphFactoryGood good;
    good.get('a')->drawIn("red");     // a in red
    good.get('a')->drawIn("blue");    // a in blue -- each use supplies its own colour
    return 0;
}
