// Anti-pattern: every state re-implementing the same default handling.
//
// Most states in a real machine care about a few events and reject the rest.
// With a pure-virtual base, every state MUST implement every event, so each one
// copies the same "not allowed here" for the events it ignores -- and adding an
// event forces an edit to every state, even the ones that only reject it. The
// fix is a base class that provides a default (here, reject) as a NON-pure
// virtual, so each state overrides only the events it actually acts on and
// inherits the rest.

#include <iostream>

// ---------- BAD: pure-virtual base forces boilerplate in every state ----------
struct StateBad {
    virtual ~StateBad() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void send() = 0;
    virtual const char* name() const = 0;
};
struct ClosedBad : StateBad {
    void open() override  { std::cout << "open\n"; }
    void close() override { std::cout << "reject: already closed\n"; } // boilerplate
    void send() override  { std::cout << "reject: not open\n"; }       // boilerplate
    const char* name() const override { return "Closed"; }
};
struct OpenBad : StateBad {
    void open() override  { std::cout << "reject: already open\n"; }   // boilerplate
    void close() override { std::cout << "close\n"; }
    void send() override  { std::cout << "send\n"; }
    const char* name() const override { return "Open"; }
};
// Adding flush() means adding a pure virtual -> editing EVERY state above.

// ---------- GOOD: base supplies the default; states override only what differs ----------
struct StateGood {
    virtual ~StateGood() = default;
    virtual void open()  { std::cout << "reject: cannot open here\n"; }  // default = reject
    virtual void close() { std::cout << "reject: cannot close here\n"; }
    virtual void send()  { std::cout << "reject: cannot send here\n"; }
    virtual const char* name() const = 0;
};
struct ClosedGood : StateGood {
    void open() override { std::cout << "open\n"; } // only the event it acts on
    const char* name() const override { return "Closed"; }
};
struct OpenGood : StateGood {
    void close() override { std::cout << "close\n"; }
    void send() override  { std::cout << "send\n"; }
    const char* name() const override { return "Open"; }
};
// Adding flush() means one default in the base; only states that flush override it.

int main() {
    std::cout << "--- bad ---\n";
    ClosedBad cb; OpenBad ob;
    cb.send();  // reject: not open
    cb.open();  // open
    ob.send();  // send
    ob.open();  // reject: already open

    std::cout << "--- good ---\n";
    ClosedGood cg; OpenGood og;
    cg.send();  // inherited default: reject
    cg.open();  // open
    og.send();  // send
    og.open();  // inherited default: reject
    return 0;
}
