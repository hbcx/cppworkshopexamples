#include <iostream>
#include <map>
#include <memory>

// The flyweight: holds intrinsic state (here just the character). Built once per
// distinct character and shared by every use of that character.
class Glyph {
public:
    explicit Glyph(char c) : c_(c) { ++totalBuilt; }
    char symbol() const { return c_; }
    static int totalBuilt;
private:
    char c_;
};
int Glyph::totalBuilt = 0;

// Factory: the heart of Flyweight. It interns glyphs by key, so a request for a
// character already built returns the SAME shared object instead of a new one.
class GlyphFactory {
public:
    std::shared_ptr<Glyph> get(char c) {
        auto it = pool_.find(c);
        if (it != pool_.end()) return it->second;   // share the existing one
        auto g = std::make_shared<Glyph>(c);
        pool_[c] = g;
        return g;
    }
private:
    std::map<char, std::shared_ptr<Glyph>> pool_;
};

int main() {
    GlyphFactory factory;
    auto a1 = factory.get('a');
    auto b  = factory.get('b');
    auto a2 = factory.get('a');   // same 'a' -> the same shared object

    std::cout << "shared glyph symbol: " << a1->symbol() << "\n";
    std::cout << "a1 and a2 are the same object: " << (a1 == a2 ? "yes" : "no") << "\n";
    std::cout << "a1 and b are the same object: " << (a1 == b ? "yes" : "no") << "\n";
    std::cout << "glyphs actually built: " << Glyph::totalBuilt << "\n";   // 2, not 3
    return 0;
}
