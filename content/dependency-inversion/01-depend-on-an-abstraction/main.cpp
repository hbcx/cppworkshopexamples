// Dependency Inversion -- a high-level module depends on an abstraction, not on
// a concrete low-level class.
//
// NotificationService is policy; how a message goes out is a detail. It depends
// on a MessageSender interface, so EmailSender and SmsSender both slot in and
// the policy never names either. The dependency on the detail is turned around:
// both the policy and each sender point at the abstraction between them.

#include <iostream>
#include <string>

// The abstraction the high-level module depends on.
class MessageSender {
public:
    virtual ~MessageSender() {}
    virtual void send(const std::string& to, const std::string& body) = 0;
};

// Low-level details -- each conforms to the abstraction.
class EmailSender : public MessageSender {
public:
    void send(const std::string& to, const std::string& body) override {
        std::cout << "[email] to " << to << ": " << body << "\n";
    }
};

class SmsSender : public MessageSender {
public:
    void send(const std::string& to, const std::string& body) override {
        std::cout << "[sms] to " << to << ": " << body << "\n";
    }
};

// High-level policy: depends only on MessageSender, never on a concrete sender.
class NotificationService {
public:
    explicit NotificationService(MessageSender& sender) : sender_(sender) {}
    void notify(const std::string& user, const std::string& msg) {
        sender_.send(user, msg);
    }
private:
    MessageSender& sender_;
};

int main() {
    EmailSender email;
    NotificationService viaEmail(email);
    viaEmail.notify("ada", "your order shipped");

    // A different channel: the SAME policy, no change to NotificationService.
    SmsSender sms;
    NotificationService viaSms(sms);
    viaSms.notify("grace", "your code is 4821");
}
