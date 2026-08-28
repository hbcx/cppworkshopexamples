#include <iostream>

// Anti-pattern: two plain enums that share an enumerator name in the same scope.
// Because unscoped enumerators leak into the enclosing scope, the second
// definition collides with the first and does not compile. Scoped enums keep
// their names inside the type, so the clash disappears.

// Unscoped: both would put "Active" into this scope -- a redefinition:
//     enum ConnState { Idle, Active, Closed };
//     enum Job       { Queued, Active, Done };   // error: 'Active' redefined
//
// The workaround people reach for is ugly prefixes on every enumerator:
enum ConnState { Conn_Idle, Conn_Active, Conn_Closed };
enum Job       { Job_Queued, Job_Active, Job_Done };

// THE FIX: scoped enums. The names live inside the type, so both can have
// "Active" with no conflict and no prefixes.
enum class ConnState2 { Idle, Active, Closed };
enum class Job2       { Queued, Active, Done };

int main() {
    std::cout << "prefixed unscoped: Conn_Active=" << Conn_Active
              << " Job_Active=" << Job_Active << "\n";              // 1 1

    ConnState2 cs = ConnState2::Active;
    Job2       jb = Job2::Active;
    std::cout << "scoped, no clash:  ConnState2::Active="
              << static_cast<int>(cs) << " Job2::Active="
              << static_cast<int>(jb) << "\n";                       // 1 1
    return 0;
}
