// Behaviour and legality vary per state: a media player.
//
// The same three commands -- play, pause, stop -- mean different things in each
// state, and some are illegal (you cannot pause a stopped player). Instead of a
// switch on a status field inside every method, each state is a class that
// answers the commands its own way and decides the transitions. The context
// (Player) holds the state and a little data (the playback position) that the
// states read and write; the states carry no context data of their own.

#include <iostream>
#include <memory>

class Player;

struct State {
    virtual ~State() = default;
    virtual std::unique_ptr<State> play(Player&) = 0;
    virtual std::unique_ptr<State> pause(Player&) = 0;
    virtual std::unique_ptr<State> stop(Player&) = 0;
    virtual void tick(Player&) = 0; // advance only if actually playing
    virtual const char* name() const = 0;
};

struct Stopped;
struct Playing;
struct Paused;

class Player {
    std::unique_ptr<State> state_;
    int pos_ = 0;
public:
    Player();
    void play()  { if (auto n = state_->play(*this))  state_ = std::move(n); }
    void pause() { if (auto n = state_->pause(*this)) state_ = std::move(n); }
    void stop()  { if (auto n = state_->stop(*this))  state_ = std::move(n); }
    void tick()  { state_->tick(*this); }
    void setPos(int p) { pos_ = p; }
    int  pos() const { return pos_; }
    const char* state() const { return state_->name(); }
};

struct Stopped : State {
    std::unique_ptr<State> play(Player& p) override; // start from 0 -> Playing
    std::unique_ptr<State> pause(Player&) override {
        std::cout << "pause -> nothing to pause (stopped)\n";
        return nullptr;
    }
    std::unique_ptr<State> stop(Player&) override {
        std::cout << "stop  -> already stopped\n";
        return nullptr;
    }
    void tick(Player&) override {} // stopped: no progress
    const char* name() const override { return "Stopped"; }
};

struct Playing : State {
    std::unique_ptr<State> play(Player&) override {
        std::cout << "play  -> already playing\n";
        return nullptr;
    }
    std::unique_ptr<State> pause(Player& p) override; // -> Paused, keep position
    std::unique_ptr<State> stop(Player& p) override;  // -> Stopped, rewind
    void tick(Player& p) override { p.setPos(p.pos() + 1); }
    const char* name() const override { return "Playing"; }
};

struct Paused : State {
    std::unique_ptr<State> play(Player&) override {
        std::cout << "play  -> resume\n";
        return std::unique_ptr<State>(new Playing());
    }
    std::unique_ptr<State> pause(Player&) override {
        std::cout << "pause -> already paused\n";
        return nullptr;
    }
    std::unique_ptr<State> stop(Player& p) override {
        std::cout << "stop  -> stop from pause, rewind\n";
        p.setPos(0);
        return std::unique_ptr<State>(new Stopped());
    }
    void tick(Player&) override {} // paused: frozen
    const char* name() const override { return "Paused"; }
};

std::unique_ptr<State> Stopped::play(Player& p) {
    std::cout << "play  -> start from 0\n";
    p.setPos(0);
    return std::unique_ptr<State>(new Playing());
}
std::unique_ptr<State> Playing::pause(Player& p) {
    std::cout << "pause -> hold at " << p.pos() << "\n";
    return std::unique_ptr<State>(new Paused());
}
std::unique_ptr<State> Playing::stop(Player& p) {
    std::cout << "stop  -> stop and rewind\n";
    p.setPos(0);
    return std::unique_ptr<State>(new Stopped());
}

Player::Player() : state_(new Stopped()) {}

int main() {
    Player p;
    std::cout << "[" << p.state() << "]\n";
    p.pause();                  // illegal while stopped
    p.play();                   // -> Playing
    p.tick(); p.tick();         // advance to 2
    p.pause();                  // -> Paused at 2
    std::cout << "[" << p.state() << " pos=" << p.pos() << "]\n";
    p.tick();                   // frozen while paused
    p.play();                   // resume -> Playing
    p.tick();                   // advance to 3
    p.stop();                   // -> Stopped, rewind
    std::cout << "[" << p.state() << " pos=" << p.pos() << "]\n";
    return 0;
}
