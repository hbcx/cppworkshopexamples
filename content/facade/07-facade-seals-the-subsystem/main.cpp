#include <iostream>
#include <string>

// ANTI-PATTERN: the facade SEALS its subsystem -- the parts are private nested
// classes reachable only through this facade. It looks tidy, but the only way to
// do anything the facade did not foresee is to widen the facade. A watermark?
// Add a method. A different order? Add a method. The facade grows one method per
// case until it is the whole subsystem API with an extra layer in front.
class SealedExporter {
public:
    std::string exportDoc(const std::string& doc) {
        return compress(render(doc));
    }
    // Added later only because nothing else can reach the parts:
    std::string exportWatermarked(const std::string& doc, const std::string& mark) {
        return compress(stamp(render(doc), mark));
    }
private:
    std::string render(const std::string& d) { std::cout << "render\n"; return d + "[r]"; }
    std::string stamp(const std::string& p, const std::string& m) { std::cout << "stamp\n"; return p + "[" + m + "]"; }
    std::string compress(const std::string& p) { std::cout << "compress\n"; return p + "[z]"; }
};

// FIX: keep the subsystem parts public and let the facade be a convenience over
// them. The common path goes through the facade; an unforeseen case uses the
// parts directly, with no need to grow the facade at all.
class Renderer {
public:
    std::string render(const std::string& d) const { std::cout << "render\n"; return d + "[r]"; }
};
class Watermark {
public:
    std::string stamp(const std::string& p, const std::string& m) const { std::cout << "stamp\n"; return p + "[" + m + "]"; }
};
class Compressor {
public:
    std::string compress(const std::string& p) const { std::cout << "compress\n"; return p + "[z]"; }
};

class OpenExporter {
public:
    std::string exportDoc(const std::string& doc) { return compressor_.compress(renderer_.render(doc)); }
private:
    Renderer renderer_;
    Compressor compressor_;
};

int main() {
    std::cout << "== sealed: every new case needs a new facade method ==\n";
    SealedExporter sealed;
    std::cout << sealed.exportWatermarked("contract", "SECRET") << "\n\n";

    std::cout << "== open: an unforeseen case uses the parts directly ==\n";
    OpenExporter open;
    std::cout << open.exportDoc("report") << "\n";
    // The watermark case the facade does not cover -- and no facade change needed:
    Renderer r;
    Watermark w;
    Compressor c;
    std::cout << c.compress(w.stamp(r.render("contract"), "SECRET")) << "\n";
    return 0;
}
