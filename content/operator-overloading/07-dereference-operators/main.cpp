#include <iostream>
#include <string>

struct User {
    std::string name;
    int age;
};

// A non-owning view that behaves like a pointer to a User. It could add checking,
// logging, or reference counting; here it just forwards, to show the two
// operators that make ptr-> and *ptr work.

class UserHandle {
public:
    explicit UserHandle(User* p) : p_(p) {}

    // *handle -> a reference to the User.
    User& operator*() const { return *p_; }

    // handle->field: return a raw pointer, and the compiler follows the arrow for
    // you, so handle->name means (handle.operator->())->name.
    User* operator->() const { return p_; }

private:
    User* p_;
};

int main() {
    User u{"Ada", 36};
    UserHandle h{&u};

    // Arrow reaches members as if h were a User*:
    std::cout << h->name << " is " << h->age << "\n";   // Ada is 36

    // Dereference gives the object itself, here used to modify it:
    (*h).age = 37;
    std::cout << h->name << " is now " << h->age << "\n";  // Ada is now 37
    return 0;
}
