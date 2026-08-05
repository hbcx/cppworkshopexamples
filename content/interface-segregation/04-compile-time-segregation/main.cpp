// Interface Segregation at the limit: a template requires exactly the
// operations it calls -- nothing more.
//
// logLine is written against a Sink type and calls one method, write(). So its
// whole requirement is "has write()". Console and Buffer satisfy it without
// inheriting anything; a type with extra methods would not make logLine depend
// on them, because the template never names them. The required interface is the
// exact set of operations used, checked at compile time.

#include <iostream>
#include <string>

// The implicit interface is one operation: write().
template <class Sink>
void logLine(Sink& sink, const std::string& message) {
    sink.write("[log] " + message);
}

// Neither type inherits anything -- each just HAS a write().
struct Console {
    void write(const std::string& s) { std::cout << s << "\n"; }
};

struct Buffer {
    std::string data;
    void write(const std::string& s) { data += s + "\n"; }
};

int main() {
    Console console;
    logLine(console, "starting up");
    logLine(console, "ready");

    Buffer buffer;
    logLine(buffer, "captured 1");
    logLine(buffer, "captured 2");
    std::cout << "buffer holds:\n" << buffer.data;
}
