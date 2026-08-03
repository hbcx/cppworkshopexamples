#include <iostream>
#include <memory>
#include <utility>

// Subject: the interface the client codes against, identical on both sides of
// the wire.
struct Calculator {
    virtual int add(int a, int b) = 0;
    virtual ~Calculator() = default;
};

// The "remote" implementation -- imagine it living in another process or on
// another host. The client never holds one of these directly.
class RemoteCalculatorServer : public Calculator {
public:
    int add(int a, int b) override { return a + b; }
};

// Remote proxy (a stub): a LOCAL stand-in with the same interface. It hides the
// wire -- it marshals the call, sends it, and receives the reply -- so the client
// makes an ordinary method call and never sees the transport. The network is
// simulated in-process here, but the shape is exactly that of an RPC stub.
class CalculatorStub : public Calculator {
public:
    explicit CalculatorStub(std::unique_ptr<Calculator> server) : server_(std::move(server)) {}
    int add(int a, int b) override {
        std::cout << "stub: marshalling add(" << a << ", " << b << ") -> network\n";
        const int result = server_->add(a, b);      // stands in for the round-trip
        std::cout << "stub: received reply " << result << "\n";
        return result;
    }
private:
    std::unique_ptr<Calculator> server_;
};

int main() {
    // The client is handed a Calculator and does not know it is remote.
    std::unique_ptr<Calculator> calc =
        std::make_unique<CalculatorStub>(std::make_unique<RemoteCalculatorServer>());
    std::cout << "result: " << calc->add(2, 3) << "\n";
    return 0;
}
