#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

// The user field sits before id, so &session->id is NOT the address of the
// whole Session -- an aliasing shared_ptr to it has a different get().
struct Session {
    std::string user;
    int id;
    Session(std::string u, int i) : user(std::move(u)), id(i) {}
};

int main() {
    auto a = std::make_shared<Session>("alice", 1);
    auto b = std::make_shared<Session>("bob", 2);

    // Value identity vs owner identity. An aliasing shared_ptr points at a's id
    // member (get() differs) yet SHARES a's control block (same owner).
    std::shared_ptr<int> a_alias(a, &a->id);
    std::cout << "same get()?   "
              << (static_cast<const void*>(a.get()) == static_cast<const void*>(a_alias.get())
                  ? "yes" : "no") << '\n';                        // no -- different pointers
    bool same_owner = !a.owner_before(a_alias) && !a_alias.owner_before(a);
    std::cout << "same owner?   " << (same_owner ? "yes" : "no") << '\n';   // yes

    // A weak_ptr has no get() to compare, so value ordering is impossible and
    // std::less<weak_ptr> would not compile. owner_less orders by control block,
    // which is stable even after the object dies -- so a weak_ptr can be a key.
    std::map<std::weak_ptr<Session>, std::string,
             std::owner_less<std::weak_ptr<Session>>> notes;
    notes[a] = "first";     // shared_ptr converts to the weak_ptr key
    notes[b] = "second";

    // Look a up via a weak_ptr made from the shared_ptr: found by shared
    // ownership, not by address.
    std::weak_ptr<Session> key_a = a;
    std::cout << "lookup a ->   " << notes[key_a] << '\n';        // first
    std::cout << "entries:      " << notes.size() << '\n';        // 2

    // Let b expire. Its key stays valid and correctly ordered -- owner ordering
    // never touches the (now dead) object, so the map is not corrupted; the dead
    // entry simply sits there until purged.
    b.reset();
    int expired = 0;
    for (const auto& kv : notes)
        if (kv.first.expired()) ++expired;
    std::cout << "expired keys: " << expired << '\n';             // 1
    std::cout << "entries:      " << notes.size() << '\n';        // still 2 until purged
    return 0;
}
