#include <iostream>
#include <vector>

// ---- PUSH model: the subject sends the changed data WITH the notification ----
struct PushObserver {
    virtual void onChanged(int temperature, int humidity) = 0;
    virtual ~PushObserver() = default;
};

class PushStation {
public:
    void subscribe(PushObserver* o) { obs_.push_back(o); }
    void update(int t, int h) {
        t_ = t;
        h_ = h;
        for (auto* o : obs_) o->onChanged(t_, h_);      // hand over the new values
    }
private:
    int t_ = 0;
    int h_ = 0;
    std::vector<PushObserver*> obs_;
};

struct TempWatcher : PushObserver {
    void onChanged(int t, int /*h*/) override { std::cout << "push: temp " << t << "\n"; }
};

// ---- PULL model: the subject says only "I changed"; the observer asks back ----
class PullStation;
struct PullObserver {
    virtual void onChanged(const PullStation& s) = 0;
    virtual ~PullObserver() = default;
};

class PullStation {
public:
    void subscribe(PullObserver* o) { obs_.push_back(o); }
    void update(int t, int h);
    int temperature() const { return t_; }
    int humidity() const { return h_; }
private:
    int t_ = 0;
    int h_ = 0;
    std::vector<PullObserver*> obs_;
};

struct TempPuller : PullObserver {
    void onChanged(const PullStation& s) override;   // reads only what it needs
};

void PullStation::update(int t, int h) {
    t_ = t;
    h_ = h;
    for (auto* o : obs_) o->onChanged(*this);         // just signal; carry no data
}

void TempPuller::onChanged(const PullStation& s) {
    std::cout << "pull: temp " << s.temperature() << "\n";
}

int main() {
    PushStation push;
    TempWatcher pw;
    push.subscribe(&pw);
    push.update(21, 55);       // observer is handed both values, uses the one it wants

    PullStation pull;
    TempPuller pp;
    pull.subscribe(&pp);
    pull.update(21, 55);       // observer is told "changed", then pulls the temperature
    return 0;
}
