#include <iostream>
#include <string>

// The builder interface: the construction STEPS, with no idea what representation
// they produce.
struct ReportBuilder {
    virtual void addTitle(const std::string& title) = 0;
    virtual void addSection(const std::string& name, const std::string& body) = 0;
    virtual std::string result() const = 0;
    virtual ~ReportBuilder() = default;
};

// One concrete builder per representation. Same steps, different output.
class HtmlReportBuilder : public ReportBuilder {
public:
    void addTitle(const std::string& title) override {
        out_ += "<h1>" + title + "</h1>\n";
    }
    void addSection(const std::string& name, const std::string& body) override {
        out_ += "<h2>" + name + "</h2>\n<p>" + body + "</p>\n";
    }
    std::string result() const override { return out_; }
private:
    std::string out_;
};

class TextReportBuilder : public ReportBuilder {
public:
    void addTitle(const std::string& title) override {
        out_ += title + "\n" + std::string(title.size(), '=') + "\n";
    }
    void addSection(const std::string& name, const std::string& body) override {
        out_ += "\n## " + name + "\n" + body + "\n";
    }
    std::string result() const override { return out_; }
private:
    std::string out_;
};

// The Director owns the recipe: the ORDER of steps, written once and reused for
// any builder. It never names a concrete representation.
struct ReportDirector {
    static void buildStandardReport(ReportBuilder& b) {
        b.addTitle("Quarterly Report");
        b.addSection("Revenue", "Up 12% over the previous quarter.");
        b.addSection("Outlook", "Steady growth expected.");
    }
};

int main() {
    // Same construction process, two representations.
    HtmlReportBuilder html;
    ReportDirector::buildStandardReport(html);
    std::cout << "--- HTML ---\n" << html.result();

    TextReportBuilder text;
    ReportDirector::buildStandardReport(text);
    std::cout << "--- TEXT ---\n" << text.result();
    return 0;
}
