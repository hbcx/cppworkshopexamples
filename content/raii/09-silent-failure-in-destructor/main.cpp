// Anti-pattern: putting fallible work ONLY in the destructor.
//
// A destructor cannot return a value and must not let an exception escape, so
// it has no way to report failure. That is fine for releasing memory, which
// cannot fail. But flushing a buffer, committing a transaction, or closing a
// file CAN fail -- and if that work happens only in the destructor, a failure
// is silently swallowed and the caller believes it succeeded. std::ofstream has
// this trap: its destructor flushes, but a write error there is lost unless you
// call close() yourself and check the stream.
//
// The fix: expose an explicit flush()/close()/commit() that REPORTS the error
// and is meant to be called on the success path. Let the destructor keep
// flushing as a last-resort backstop, but do not rely on it to tell you whether
// the data made it out.

#include <iostream>
#include <string>

// A buffered writer. "Flushing" can fail (disk full, broken pipe, ...).
class BufferedWriter {
public:
    explicit BufferedWriter(bool diskWillFail) : diskWillFail_(diskWillFail) {}

    void write(const std::string& s) { buffer_ += s; }

    // Explicit flush that REPORTS success or failure. Call it and check it.
    bool flush() {
        if (flushed_) return true;
        flushed_ = true;
        if (diskWillFail_) {
            std::cout << "flush: FAILED to write " << buffer_.size() << " bytes\n";
            return false;               // caller can react: retry, alert, abort
        }
        std::cout << "flush: wrote " << buffer_.size() << " bytes\n";
        return true;
    }

    ~BufferedWriter() {
        if (!flushed_) {
            // Backstop only. If this fails we can neither return nor throw, so
            // the failure is invisible -- which is why it must not be the ONLY
            // place the flush happens.
            bool ok = flush();
            std::cout << "~BufferedWriter: backstop flush ok=" << ok
                      << " (caller never saw this result)\n";
        }
    }
private:
    std::string buffer_;
    bool diskWillFail_;
    bool flushed_ = false;
};

int main() {
    std::cout << "-- BAD: rely on the destructor to flush --\n";
    {
        BufferedWriter w(true);          // the disk will fail
        w.write("important data");
        // No flush() call. The failure happens in the destructor and is lost;
        // this code path believes the write succeeded.
        std::cout << "caller: done (believes the write succeeded)\n";
    }

    std::cout << "\n-- GOOD: flush explicitly and check --\n";
    {
        BufferedWriter w(true);          // same failing disk
        w.write("important data");
        if (!w.flush()) {
            std::cout << "caller: flush failed -- retrying / alerting\n";
        }
    }
}
