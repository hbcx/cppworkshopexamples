// RAII in one idea: tie a resource to an object's lifetime. The constructor
// acquires the resource; the destructor releases it. When the object goes out
// of scope the language runs the destructor for you -- so the release is
// automatic and happens at a known point, with no cleanup call to remember.
//
// Here the "resource" is a stand-in file handle. Opening prints a line and
// closing prints a line, so you can see exactly when each happens.

#include <iostream>
#include <string>
#include <utility>

class FileHandle {
public:
    explicit FileHandle(std::string name) : name_(std::move(name)) {
        std::cout << "open  " << name_ << "\n";   // acquire the resource
    }
    ~FileHandle() {
        std::cout << "close " << name_ << "\n";   // release it -- runs automatically
    }
    void write(const std::string& text) {
        std::cout << "write " << name_ << ": " << text << "\n";
    }
private:
    std::string name_;
};

int main() {
    std::cout << "-- enter block --\n";
    {
        FileHandle log("app.log");
        log.write("started");
        // No close() call. The destructor closes app.log at the closing brace,
        // below, even if we had returned early or thrown from inside the block.
    }
    std::cout << "-- left block --\n";

    // Two handles in one scope: destructors run in REVERSE order of
    // construction, so b (built last) is closed before a.
    FileHandle a("a.txt");
    FileHandle b("b.txt");
    a.write("hi");
}
