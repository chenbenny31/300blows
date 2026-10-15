// B19 (no direct LC; 706 set basics): filter an integer stream against a blacklist that changes at runtime;
// follow-up: block / unblock report a state-change event when an already-seen value flips visible <-> hidden.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;

class BlacklistFilter {
public:
    // hash set lookup, T: O(1) average, S: O(B + U), B: blocked values, U: distinct values seen
    // true = x passes (emitted); every occurrence is judged, repeats pass each time
    bool process(int x) {
        seen.insert(x);
        return blocked.count(x) == 0;
    }

    // T: O(1) average; true = event "x is now hidden", only for a seen value that was visible
    bool block(int x) {
        bool changed = blocked.insert(x).second;   // false: already blocked, no change
        return changed && seen.count(x) != 0;
    }

    // T: O(1) average; true = event "x is visible again", only for a seen value that was hidden
    bool unblock(int x) {
        bool changed = blocked.erase(x) > 0;       // false: was not blocked, no change
        return changed && seen.count(x) != 0;
    }

private:
    unordered_set<int> blocked;   // values currently on the blacklist
    unordered_set<int> seen;      // values that have appeared in the stream (only for events)
};

bool check(bool got, bool expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool testFilter() {
    bool ok = true;
    BlacklistFilter f;
    ok &= check(f.process(1), true, "process(1) empty blacklist");
    ok &= check(f.process(2), true, "process(2)");
    f.block(1);
    ok &= check(f.process(1), false, "process(1) blocked");
    ok &= check(f.process(2), true, "process(2) other value unaffected");
    f.unblock(1);
    ok &= check(f.process(1), true, "process(1) after unblock");
    ok &= check(f.process(1), true, "process(1) repeat passes again");
    f.block(-3);   // negative values are ordinary keys
    ok &= check(f.process(-3), false, "process(-3) blocked before first seen");
    return ok;
}

bool testEvents() {
    bool ok = true;
    BlacklistFilter f;
    ok &= check(f.process(5), true, "process(5)");
    ok &= check(f.block(5), true, "block(5) seen + visible -> event");
    ok &= check(f.block(5), false, "block(5) repeat, no change");
    ok &= check(f.unblock(5), true, "unblock(5) -> visible event");
    ok &= check(f.unblock(5), false, "unblock(5) repeat, no change");
    ok &= check(f.block(7), false, "block(7) never seen, silent");
    ok &= check(f.process(7), false, "process(7) blocked");
    ok &= check(f.unblock(7), true, "unblock(7) seen while blocked -> event");
    ok &= check(f.block(8), false, "block(8) never seen");
    ok &= check(f.unblock(8), false, "unblock(8) never seen, round trip silent");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testFilter();   // pass / block / unblock / repeat inputs
    ok &= testEvents();   // follow-up: events only for seen values whose visibility flips
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
