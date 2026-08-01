#include <iostream>
#include <memory>
#include <string>

// Anti-pattern: clone() runs the default copy constructor, which copies the
// shared_ptr HANDLE -- so the clone points at the same string as the original.
class ShallowNode {
public:
    explicit ShallowNode(std::string t)
        : text_(std::make_shared<std::string>(std::move(t))) {}
    std::unique_ptr<ShallowNode> clone() const {
        return std::make_unique<ShallowNode>(*this);   // shallow: shares text_
    }
    void append(char c) { text_->push_back(c); }
    const std::string& text() const { return *text_; }
private:
    std::shared_ptr<std::string> text_;
};

// Fix: clone() deep-copies the resource into a fresh handle, so the clone owns
// its own state and is independent.
class DeepNode {
public:
    explicit DeepNode(std::string t)
        : text_(std::make_shared<std::string>(std::move(t))) {}
    std::unique_ptr<DeepNode> clone() const {
        return std::make_unique<DeepNode>(*text_);     // deep: new string from the value
    }
    void append(char c) { text_->push_back(c); }
    const std::string& text() const { return *text_; }
private:
    std::shared_ptr<std::string> text_;
};

int main() {
    // Shallow: appending to the clone also changes the original -- they alias.
    ShallowNode a("doc");
    std::unique_ptr<ShallowNode> aClone = a.clone();
    aClone->append('!');
    std::cout << "shallow -> original: " << a.text() << "   clone: " << aClone->text() << '\n';

    // Deep: the clone owns its own state, so the original is untouched.
    DeepNode b("doc");
    std::unique_ptr<DeepNode> bClone = b.clone();
    bClone->append('!');
    std::cout << "deep    -> original: " << b.text() << "   clone: " << bClone->text() << '\n';
    return 0;
}
