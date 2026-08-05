// Interface Segregation -- split a fat interface into small role interfaces.
//
// Instead of one read-and-write interface, there are two roles: Reader and
// Writer. A TextFile fills both; a ConsoleLog only writes, so it implements
// Writer alone and is never asked for a read() it cannot mean. Functions depend
// on the narrowest role they need -- record() takes a Writer and can never read.

#include <iostream>
#include <string>

// Two small roles instead of one fat interface.
class Reader {
public:
    virtual ~Reader() {}
    virtual std::string read() const = 0;
};

class Writer {
public:
    virtual ~Writer() {}
    virtual void write(const std::string& line) = 0;
};

// Fills both roles.
class TextFile : public Reader, public Writer {
public:
    std::string read() const override { return content_; }
    void write(const std::string& line) override { content_ += line; }
private:
    std::string content_;
};

// Fills only the Writer role -- no meaningless read() to stub.
class ConsoleLog : public Writer {
public:
    void write(const std::string& line) override {
        std::cout << "[log] " << line << "\n";
    }
};

// Each function depends on exactly the role it uses.
void dump(const Reader& r) { std::cout << "file holds: " << r.read() << "\n"; }
void record(Writer& w, const std::string& msg) { w.write(msg); }

int main() {
    TextFile file;
    record(file, "hello ");   // used as a Writer
    record(file, "world");
    dump(file);               // used as a Reader

    ConsoleLog log;
    record(log, "started");   // Writer only -- dump(log) would not compile
}
