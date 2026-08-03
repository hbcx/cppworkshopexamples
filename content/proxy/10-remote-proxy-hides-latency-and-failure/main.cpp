#include <iostream>
#include <string>

// Subject with a local-looking signature: it returns an int, with no way to
// signal that a call failed.
struct Prices {
    virtual int priceOf(const std::string& sym) = 0;
    virtual ~Prices() = default;
};

// The remote endpoint. Over a real network this call can time out or fail; here
// a failure is simulated for one symbol.
class RemotePrices : public Prices {
public:
    int priceOf(const std::string& sym) override {
        if (sym == "DOWN") { failed_ = true; return 0; }    // simulated network error
        failed_ = false;
        return static_cast<int>(sym.size()) * 100;
    }
    bool failed() const { return failed_; }
private:
    bool failed_ = false;
};

// ANTI-PATTERN: the remote proxy mirrors a LOCAL interface exactly -- priceOf
// returns a bare int. When the remote call fails there is nowhere to say so, so
// the proxy returns 0, which the caller cannot tell apart from a real price of 0.
// Remote calls fail and stall in ways local calls never do; hiding that behind a
// local-looking signature is a leaky abstraction that loses errors silently.
class TransparentStub : public Prices {
public:
    explicit TransparentStub(RemotePrices& r) : remote_(r) {}
    int priceOf(const std::string& sym) override { return remote_.priceOf(sym); }
private:
    RemotePrices& remote_;
};

// FIX: the remote proxy surfaces failure. Its result type carries an ok flag, so
// the caller has to acknowledge that a remote call can fail and cannot mistake an
// error for a value. (A real one would also expose timeouts and retries.)
struct Quote {
    bool ok;
    int price;
};

class HonestStub {
public:
    explicit HonestStub(RemotePrices& r) : remote_(r) {}
    Quote priceOf(const std::string& sym) {
        const int p = remote_.priceOf(sym);
        return Quote{!remote_.failed(), p};
    }
private:
    RemotePrices& remote_;
};

int main() {
    RemotePrices remote;

    std::cout << "== transparent stub (loses the error) ==\n";
    TransparentStub bad(remote);
    std::cout << "AAA  -> " << bad.priceOf("AAA") << "\n";
    std::cout << "DOWN -> " << bad.priceOf("DOWN") << "  (a real price, or a failure?)\n";

    std::cout << "== honest stub (surfaces the error) ==\n";
    HonestStub good(remote);
    const Quote q1 = good.priceOf("AAA");
    std::cout << "AAA  -> ok=" << (q1.ok ? "true" : "false") << " price=" << q1.price << "\n";
    const Quote q2 = good.priceOf("DOWN");
    std::cout << "DOWN -> ok=" << (q2.ok ? "true" : "false") << " price=" << q2.price << "\n";
    return 0;
}
