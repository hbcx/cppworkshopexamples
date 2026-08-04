// Anti-pattern: a colleague destroyed while still registered.
//
// The mediator holds raw pointers to its colleagues so it can reach them. If a
// colleague is destroyed but never removed from the mediator, the mediator keeps
// a dangling pointer, and the next interaction dereferences freed memory. This is
// the same lifetime bug the Observer chapter has, on the mediator side. The
// dangling case is described in the comment (undefined behaviour, not run). The
// fix shown here is RAII: a colleague unregisters itself in its destructor, so
// the mediator never holds a pointer to a dead colleague.
//
// ---- BAD (undefined behaviour -- described, NOT run) ----
//
//   Room room;
//   {
//       Member temp("Temp", room);   // registers itself with the room
//   }                                // temp destroyed, but still in room's list
//   room.broadcast("hi", nullptr);   // dereferences the dangling Member* -> UAF
//
// Nothing removed temp from the room, so room.members_ holds a pointer into
// freed memory.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

class Member;

class Room { // the mediator
    std::vector<Member*> members_;
public:
    void join(Member* m) { members_.push_back(m); }
    void leave(Member* m) {
        members_.erase(std::remove(members_.begin(), members_.end(), m), members_.end());
    }
    void broadcast(const std::string& msg, const Member* from);
};

class Member {
    std::string name_;
    Room& room_;
public:
    Member(std::string name, Room& room) : name_(std::move(name)), room_(room) {
        room_.join(this);
    }
    ~Member() { room_.leave(this); } // RAII: unregister on destruction
    const std::string& name() const { return name_; }
    void receive(const std::string& msg) const {
        std::cout << "  " << name_ << " got: " << msg << "\n";
    }
};

void Room::broadcast(const std::string& msg, const Member* from) {
    for (Member* m : members_)
        if (m != from)
            m->receive(msg);
}

int main() {
    Room room;
    Member alice("Alice", room);
    {
        Member guest("Guest", room);
        std::cout << "with guest present:\n";
        room.broadcast("hello", nullptr);
    } // guest destroyed here -> its destructor removes it from the room

    std::cout << "after guest left:\n";
    room.broadcast("still here?", nullptr); // safe: room no longer holds Guest
    return 0;
}
