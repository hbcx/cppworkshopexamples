#include <iostream>
#include <string>

// Subsystem parts. All public and usable on their own -- a facade over them does
// not make them private or off-limits.
class Renderer {
public:
    std::string render(const std::string& doc) const {
        std::cout << "render: laying out " << doc << '\n';
        return doc + "[rendered]";
    }
};

class Watermark {
public:
    std::string stamp(const std::string& page, const std::string& mark) const {
        std::cout << "watermark: stamping " << mark << '\n';
        return page + "[" + mark + "]";
    }
};

class Compressor {
public:
    std::string compress(const std::string& page) const {
        std::cout << "compress: shrinking\n";
        return page + "[zip]";
    }
};

// Facade: the common export path -- render then compress -- in one call. It
// covers most needs, but deliberately does not expose every subsystem feature.
class PdfExporter {
public:
    std::string exportDoc(const std::string& doc) {
        return compressor_.compress(renderer_.render(doc));
    }
private:
    Renderer renderer_;
    Compressor compressor_;
};

int main() {
    // The easy path: one call through the facade.
    PdfExporter exporter;
    std::cout << "== common path ==\n";
    const std::string out = exporter.exportDoc("report");
    std::cout << "result: " << out << "\n\n";

    // A facade simplifies but does NOT seal the subsystem. For a case the facade
    // does not cover -- here a watermark -- the client drops down to the
    // subsystem classes directly. They were never hidden, so the facade adds a
    // convenient default without taking any capability away.
    std::cout << "== advanced path (watermarked) ==\n";
    Renderer renderer;
    Watermark watermark;
    Compressor compressor;
    std::string page = renderer.render("contract");
    page = watermark.stamp(page, "CONFIDENTIAL");
    page = compressor.compress(page);
    std::cout << "result: " << page << '\n';
    return 0;
}
