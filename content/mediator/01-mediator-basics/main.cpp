// Mediator basics: colleagues talk through a hub, not to each other.
//
// A chat room is the classic minimal Mediator: users never hold references to
// one another, they only know the room. A user sends a message TO the room, and
// the room decides who receives it. Add or remove a user and no other user's
// code changes; change how delivery works and only the room changes. That is the
// pattern's point -- replace a mesh of user-to-user links with a single hub that
// owns the interaction.

#include <iostream>
#include <string>
#include <vector>
#include <utility>

class User;

class ChatRoom { // the mediator
    std::vector<User*> users_;
public:
    void join(User* u) { users_.push_back(u); }
    void send(const std::string& from, const std::string& msg, const User* sender);
};

class User { // a colleague: knows only the room
    std::string name_;
    ChatRoom& room_;
public:
    User(std::string name, ChatRoom& room) : name_(std::move(name)), room_(room) {
        room_.join(this);
    }
    const std::string& name() const { return name_; }
    void send(const std::string& msg) { room_.send(name_, msg, this); } // to the room
    void receive(const std::string& from, const std::string& msg) const {
        std::cout << "  [" << name_ << " sees] " << from << ": " << msg << "\n";
    }
};

void ChatRoom::send(const std::string& from, const std::string& msg, const User* sender) {
    for (User* u : users_)
        if (u != sender) // deliver to everyone except the sender
            u->receive(from, msg);
}

int main() {
    ChatRoom room;
    User alice("Alice", room);
    User bob("Bob", room);
    User carol("Carol", room);

    std::cout << "Alice: hi all\n";
    alice.send("hi all");
    std::cout << "Bob: hey Alice\n";
    bob.send("hey Alice");
    return 0;
}
