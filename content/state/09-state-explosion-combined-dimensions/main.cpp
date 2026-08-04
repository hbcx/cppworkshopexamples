// Anti-pattern: one state enum for two independent dimensions.
//
// A session here has two INDEPENDENT concerns: the link (Down/Up) and the auth
// (Anonymous/Authenticated). Encode them as one enum and you get their cross
// product -- four states -- and every action must be written once per value of
// the OTHER dimension: connect() appears for each auth value, login() for each
// link value. Add a third link state and the auth axis multiplies it again. The
// fix is to keep the two dimensions as two separate state variables, so each
// transition is defined once and the two axes stay 2 + 2, not 2 * 2.

#include <iostream>

// ---------- BAD: the cross product as a single enum ----------
enum class Combined { DownAnon, DownAuth, UpAnon, UpAuth };

const char* nameBad(Combined c) {
    switch (c) {
        case Combined::DownAnon: return "Down/Anon";
        case Combined::DownAuth: return "Down/Auth";
        case Combined::UpAnon:   return "Up/Anon";
        case Combined::UpAuth:   return "Up/Auth";
    }
    return "?";
}
Combined connectBad(Combined c) {          // "connect" -- once per auth value
    switch (c) {
        case Combined::DownAnon: return Combined::UpAnon;
        case Combined::DownAuth: return Combined::UpAuth;
        default:                 return c; // already up
    }
}
Combined loginBad(Combined c) {            // "login" -- once per link value
    switch (c) {
        case Combined::DownAnon: return Combined::DownAuth;
        case Combined::UpAnon:   return Combined::UpAuth;
        default:                 return c; // already authed
    }
}

// ---------- GOOD: two independent state variables ----------
enum class Link { Down, Up };
enum class Auth { Anon, Authed };

struct Session {
    Link link = Link::Down;
    Auth auth = Auth::Anon;
    void connect() { link = Link::Up; }     // one definition, independent of auth
    void login()   { auth = Auth::Authed; } // one definition, independent of link
};

const char* nameLink(Link l) { return l == Link::Up ? "Up" : "Down"; }
const char* nameAuth(Auth a) { return a == Auth::Authed ? "Auth" : "Anon"; }

int main() {
    std::cout << "--- bad ---\n";
    Combined c = Combined::DownAnon;
    std::cout << nameBad(c) << "\n";
    c = loginBad(c);   // authenticate first
    std::cout << nameBad(c) << "\n";
    c = connectBad(c); // then bring the link up
    std::cout << nameBad(c) << "\n";

    std::cout << "--- good ---\n";
    Session s;
    std::cout << nameLink(s.link) << "/" << nameAuth(s.auth) << "\n";
    s.login();   // touches only the auth axis
    std::cout << nameLink(s.link) << "/" << nameAuth(s.auth) << "\n";
    s.connect(); // touches only the link axis
    std::cout << nameLink(s.link) << "/" << nameAuth(s.auth) << "\n";
    return 0;
}
