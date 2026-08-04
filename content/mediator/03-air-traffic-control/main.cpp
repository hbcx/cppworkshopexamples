// The mediator as an arbiter: it holds state and makes decisions.
//
// A control tower is a Mediator that does more than relay: it owns a shared
// resource (the runway) and the logic for scheduling access to it. Planes never
// coordinate with each other -- they ask the tower to land, and the tower either
// clears them or makes them hold, then clears the next waiting plane when the
// runway frees. The coordination policy (one plane at a time, first-come order)
// lives entirely in the tower.

#include <iostream>
#include <string>
#include <deque>
#include <utility>

class Plane;

class Tower { // the mediator: owns the runway and the schedule
    bool runwayBusy_ = false;
    std::deque<Plane*> waiting_;
public:
    void requestLanding(Plane* p);
    void reportLanded(Plane* p);
};

class Plane {
    std::string id_;
    Tower& tower_;
public:
    Plane(std::string id, Tower& t) : id_(std::move(id)), tower_(t) {}
    const std::string& id() const { return id_; }
    void requestLanding() { tower_.requestLanding(this); }
    void clearedToLand() const { std::cout << "  " << id_ << " cleared to land\n"; }
    void hold() const { std::cout << "  " << id_ << " holding\n"; }
    void land() { std::cout << "  " << id_ << " landing\n"; tower_.reportLanded(this); }
};

void Tower::requestLanding(Plane* p) {
    if (!runwayBusy_) {
        runwayBusy_ = true;
        p->clearedToLand();
    } else {
        waiting_.push_back(p); // queued: the tower remembers who is waiting
        p->hold();
    }
}
void Tower::reportLanded(Plane* p) {
    std::cout << "  " << p->id() << " cleared the runway\n";
    runwayBusy_ = false;
    if (!waiting_.empty()) { // clear the next plane in order
        Plane* next = waiting_.front();
        waiting_.pop_front();
        runwayBusy_ = true;
        next->clearedToLand();
    }
}

int main() {
    Tower tower;
    Plane aa("AA1", tower), bb("BB2", tower), cc("CC3", tower);

    aa.requestLanding(); // runway free -> cleared
    bb.requestLanding(); // busy -> holds
    cc.requestLanding(); // busy -> holds

    aa.land(); // frees runway -> BB2 cleared next
    bb.land(); // frees runway -> CC3 cleared next
    cc.land(); // frees runway -> queue empty
    return 0;
}
