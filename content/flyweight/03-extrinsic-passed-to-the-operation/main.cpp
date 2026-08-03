#include <iostream>
#include <map>
#include <memory>
#include <string>

// Flyweight with INTRINSIC state only (the character). The extrinsic state --
// where to draw it -- is NOT stored on the flyweight; it is passed to the
// operation each time, so one Glyph serves every position that character appears.
class Glyph {
public:
    explicit Glyph(char c) : c_(c) { ++built; }
    void drawAt(int column) const {              // column is EXTRINSIC -- an argument
        std::cout << "col " << column << ": " << c_ << "\n";
    }
    static int built;
private:
    char c_;
};
int Glyph::built = 0;

class GlyphFactory {
public:
    const Glyph& get(char c) {
        auto it = pool_.find(c);
        if (it == pool_.end()) {
            it = pool_.emplace(c, std::make_shared<Glyph>(c)).first;
        }
        return *it->second;
    }
private:
    std::map<char, std::shared_ptr<Glyph>> pool_;
};

int main() {
    GlyphFactory glyphs;
    const std::string text = "abba";

    // Render each character at its column, passing the column as extrinsic state.
    // The flyweight stores no position, so the same 'a' object draws at columns
    // 0 and 3, and the same 'b' at columns 1 and 2.
    int col = 0;
    for (char c : text) {
        glyphs.get(c).drawAt(col);
        ++col;
    }
    std::cout << text.size() << " characters drawn using " << Glyph::built
              << " glyph objects\n";
    return 0;
}
