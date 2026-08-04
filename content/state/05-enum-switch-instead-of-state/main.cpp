// Anti-pattern: a status enum and a switch in every method.
//
// This is the design State replaces. A document tracks its lifecycle with a
// Status enum and switches on it in submit(), approve() and label() -- the same
// enum, three times. It works, and for three states it looks fine. The cost is
// that the state-dependent behaviour is scattered across the methods instead of
// gathered per state, so adding a status means finding and editing every switch
// together, and a forgotten case falls through to a wrong-but-quiet default.
// The fix (DocumentGood) is one class per state; adding a state is a new class.

#include <iostream>
#include <memory>

// ---------- BAD: one enum, a switch repeated in every method ----------
class DocumentBad {
    enum class Status { Draft, Moderation, Published } status_ = Status::Draft;
public:
    void submit() {
        switch (status_) {
            case Status::Draft:      status_ = Status::Moderation; std::cout << "submitted for review\n"; break;
            case Status::Moderation: std::cout << "already under review\n"; break;
            case Status::Published:  std::cout << "cannot submit a published document\n"; break;
        }
    }
    void approve() {
        switch (status_) {
            case Status::Draft:      std::cout << "cannot approve a draft\n"; break;
            case Status::Moderation: status_ = Status::Published; std::cout << "approved and published\n"; break;
            case Status::Published:  std::cout << "already published\n"; break;
        }
    }
    const char* label() const {
        switch (status_) {
            case Status::Draft:      return "Draft";
            case Status::Moderation: return "Moderation";
            case Status::Published:  return "Published";
        }
        return "?";
    }
};

// ---------- GOOD: one class per state, behaviour and transition together ----------
struct DocState {
    virtual ~DocState() = default;
    virtual std::unique_ptr<DocState> submit() = 0;
    virtual std::unique_ptr<DocState> approve() = 0;
    virtual const char* label() const = 0;
};

struct ModerationState;
struct PublishedState;

struct DraftState : DocState {
    std::unique_ptr<DocState> submit() override;  // -> Moderation
    std::unique_ptr<DocState> approve() override {
        std::cout << "cannot approve a draft\n";
        return nullptr;
    }
    const char* label() const override { return "Draft"; }
};
struct ModerationState : DocState {
    std::unique_ptr<DocState> submit() override {
        std::cout << "already under review\n";
        return nullptr;
    }
    std::unique_ptr<DocState> approve() override;  // -> Published
    const char* label() const override { return "Moderation"; }
};
struct PublishedState : DocState {
    std::unique_ptr<DocState> submit() override {
        std::cout << "cannot submit a published document\n";
        return nullptr;
    }
    std::unique_ptr<DocState> approve() override {
        std::cout << "already published\n";
        return nullptr;
    }
    const char* label() const override { return "Published"; }
};

std::unique_ptr<DocState> DraftState::submit() {
    std::cout << "submitted for review\n";
    return std::unique_ptr<DocState>(new ModerationState());
}
std::unique_ptr<DocState> ModerationState::approve() {
    std::cout << "approved and published\n";
    return std::unique_ptr<DocState>(new PublishedState());
}

class DocumentGood {
    std::unique_ptr<DocState> state_;
public:
    DocumentGood() : state_(new DraftState()) {}
    void submit()  { if (auto n = state_->submit())  state_ = std::move(n); }
    void approve() { if (auto n = state_->approve()) state_ = std::move(n); }
    const char* label() const { return state_->label(); }
};

int main() {
    std::cout << "--- bad ---\n";
    DocumentBad bad;
    bad.approve(); // illegal from Draft
    bad.submit();  // -> Moderation
    bad.approve(); // -> Published
    bad.submit();  // illegal
    std::cout << "label: " << bad.label() << "\n";

    std::cout << "--- good ---\n";
    DocumentGood good;
    good.approve();
    good.submit();
    good.approve();
    good.submit();
    std::cout << "label: " << good.label() << "\n";
    return 0;
}
