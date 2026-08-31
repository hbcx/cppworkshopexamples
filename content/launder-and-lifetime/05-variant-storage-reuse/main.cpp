// A fixed storage slot that holds a different Message each step. Message owns a
// std::string (so the old one must be destroyed before reuse) and has a const
// kind field (so a kept pointer must be laundered). The slot cycles with no heap
// churn for the storage itself and no undefined behaviour.

#include <iostream>
#include <new>
#include <string>
#include <utility>

struct Message {
    const int kind;      // const -> a kept pointer needs launder after reuse
    std::string text;    // owns memory -> must be destroyed before reuse
    static int alive;
    Message(int k, std::string t) : kind(k), text(std::move(t)) { ++alive; }
    ~Message() { --alive; }
};
int Message::alive = 0;

int main() {
    alignas(Message) unsigned char slot[sizeof(Message)];

    // Step 1: build the first message.
    Message* m = new (slot) Message(1, "handshake");
    std::cout << "kind " << m->kind << ": " << m->text << '\n';

    // Step 2: reuse the slot. Destroy the old message first (it owns a string),
    // then construct the next one in the same bytes.
    m->~Message();
    new (slot) Message(2, "payload");

    // We only kept m, and Message has a const member, so launder before use.
    Message* m2 = std::launder(m);
    std::cout << "kind " << m2->kind << ": " << m2->text << '\n';

    m2->~Message();

    std::cout << "live messages at the end: " << Message::alive << '\n';
}
