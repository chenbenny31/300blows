// integer stream with a dynamic blacklist (Q42, J10688)
// process(x): emit x unless it is blacklisted
// follow-up: ban / unban report a state-change event when an already-seen value flips visible <-> hidden

// repeated input: emit every time, or only the first time? (material contradicts itself)
// ban / unban of a value never seen: event or silent?
// repeated ban / unban: no new event?
// concurrent producers and blacklist updates?

#include <unordered_set>
#include <cassert>
#include <iostream>

// T: every operation O(1) average, S: O(B + S), B: blacklisted values, S: values seen

class StreamFilter {
private:
    std::unordered_set<int> blacklist; // values currently banned
    std::unordered_set<int> seen; // values that have appeared in the stream
    // visible(x) = seen(x) && !blacklist(x), no third set needed

public:
    // emits every non-banned occurrence
    bool process(int x) {
        seen.insert(x);
        return blacklist.count(x) == 0;
    }

    // true = event "x is now hidden"; only for a seen value that was visible
    bool ban(int x) {
        bool added = blacklist.insert(x).second; // false if already banned: no change
        return added && seen.count(x) != 0;
    }

    // true = event "x is visible again"; only for a seen value that was hidden
    bool unban(int x) {
        bool removed = blacklist.erase(x) > 0;
        return removed && seen.count(x) != 0;
    }
};

int main() {
    StreamFilter f;

    assert(f.process(1)); // null: empty blacklist emits
    assert(f.ban(1)); // target: seen value flips to hidden -> event
    assert(!f.process(1)); // banned value not emitted
    assert(!f.ban(1)); // edge: repeated ban, no new event

    assert(!f.ban(2)); // edge: ban before 2 is ever seen, silent
    assert(!f.process(2));
    assert(f.unban(2)); // 2 was seen while banned, now visible -> event
    assert(f.process(2)); // re-input after unban is emitted

    assert(!f.unban(3)); // fault: unban a value that was never banned or seen

    std::cout << "all tests passed\n";
    return 0;
}
