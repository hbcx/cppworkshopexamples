#include <iostream>
#include <string>
#include <utility>

// Subject.
struct Document {
    virtual void read() = 0;
    virtual void write(const std::string& text) = 0;
    virtual ~Document() = default;
};

// Real subject: it just stores and shows content. It knows nothing about
// permissions -- that concern lives entirely in the proxy.
class RealDocument : public Document {
public:
    void read() override { std::cout << "reading: " << content_ << "\n"; }
    void write(const std::string& text) override {
        content_ = text;
        std::cout << "wrote: " << text << "\n";
    }
private:
    std::string content_ = "(empty)";
};

// Protection proxy: the same Document interface, but it checks the caller's role
// before forwarding. Anyone may read; only an editor may write. The access rule
// is enforced here, so the real document stays free of authorization code.
class ProtectedDocument : public Document {
public:
    ProtectedDocument(RealDocument& doc, std::string role) : doc_(doc), role_(std::move(role)) {}
    void read() override { doc_.read(); }
    void write(const std::string& text) override {
        if (role_ != "editor") {
            std::cout << "denied: role '" << role_ << "' cannot write\n";
            return;
        }
        doc_.write(text);
    }
private:
    RealDocument& doc_;
    std::string role_;
};

int main() {
    RealDocument doc;

    ProtectedDocument asViewer(doc, "viewer");
    asViewer.read();
    asViewer.write("hello");        // denied by the proxy

    ProtectedDocument asEditor(doc, "editor");
    asEditor.write("hello");        // allowed
    asEditor.read();
    return 0;
}
