#include <iostream>
#include <vector>
#include <cstddef>

// A reusable buffer.
class ByteBuffer {
public:
    void write(int b) { bytes_.push_back(b); }
    std::size_t size() const { return bytes_.size(); }
private:
    std::vector<int> bytes_;
};

// Anti-pattern: private inheritance to "reuse" ByteBuffer. The pipe IS one buffer,
// so it can never hold a second, separate one -- it is stuck with a single stream.
class BadPipe : private ByteBuffer {
public:
    void feed(int b) { write(b); }
    std::size_t buffered() const { return size(); }
    // No way to add a distinct output buffer: the class already IS the one buffer.
};

// FIX: composition. Two named members, each an independent buffer.
class GoodPipe {
public:
    void feed(int b) { in_.write(b); }
    void emit(int b) { out_.write(b); }
    std::size_t inSize()  const { return in_.size(); }
    std::size_t outSize() const { return out_.size(); }
private:
    ByteBuffer in_;
    ByteBuffer out_;
};

int main() {
    BadPipe bad;
    bad.feed(1);
    bad.feed(2);
    std::cout << "bad pipe (one stream only): " << bad.buffered() << "\n";   // 2

    GoodPipe good;
    good.feed(1);              // input
    good.emit(7);              // output
    good.emit(8);
    std::cout << "good pipe: in = " << good.inSize()
              << ", out = " << good.outSize() << "\n";   // in = 1, out = 2
    return 0;
}
