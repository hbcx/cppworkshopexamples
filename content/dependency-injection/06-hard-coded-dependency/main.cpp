#include <iostream>
#include <string>

struct Renderer {
    virtual std::string render(const std::string& doc) = 0;
    virtual ~Renderer() = default;
};

struct PdfRenderer : Renderer {
    std::string render(const std::string& doc) override { return "PDF{" + doc + "}"; }
};

struct FakeRenderer : Renderer {
    std::string render(const std::string& doc) override { return "FAKE{" + doc + "}"; }
};

// Anti-pattern: Report constructs its own concrete PdfRenderer. There is no
// seam -- you cannot render with anything else, and a test cannot avoid the real
// (possibly slow, file-writing) renderer.
class ReportHardCoded {
public:
    std::string build(const std::string& data) {
        PdfRenderer renderer;                     // baked in
        return renderer.render("report:" + data);
    }
};

// Fix: depend on the Renderer interface and receive it. The concrete type is now
// the caller's choice, and a test injects a fake.
class ReportInjected {
public:
    explicit ReportInjected(Renderer& renderer) : renderer_(renderer) {}
    std::string build(const std::string& data) {
        return renderer_.render("report:" + data);
    }
private:
    Renderer& renderer_;
};

int main() {
    // Hard-coded: stuck with PdfRenderer, no way in for a test.
    ReportHardCoded hard;
    std::cout << "hard-coded -> " << hard.build("q3") << '\n';

    // Injected: the same report builds against a fake in a test.
    FakeRenderer fake;
    ReportInjected injected(fake);
    std::cout << "injected   -> " << injected.build("q3") << '\n';
    return 0;
}
