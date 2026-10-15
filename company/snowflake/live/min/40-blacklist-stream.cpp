// B19 · LC 706 set basics: filter an int stream against a runtime blacklist. Follow-up: block/unblock return true when a seen value flips visible <-> hidden.
#include <iostream>
#include <string>
#include <unordered_set>
using std::cout;
using std::string;
using std::unordered_set;

// two hash sets, every op T: O(1) average, S: O(B + U)
class BlacklistFilter {
public:
    // true = x passes; every occurrence is judged, repeats pass each time
    bool process(int x) {
        seen.insert(x);
        return blocked.count(x) == 0;
    }

    // true = event "x is now hidden"
    bool block(int x) {
        bool changed = blocked.insert(x).second;   // false: already blocked
        return changed && seen.count(x) != 0;
    }

    // true = event "x is visible again"
    bool unblock(int x) {
        bool changed = blocked.erase(x) > 0;       // false: was not blocked
        return changed && seen.count(x) != 0;
    }

private:
    unordered_set<int> blocked;   // values currently on the blacklist
    unordered_set<int> seen;      // values that have appeared in the stream (events only)
};

void check(bool got, bool expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    BlacklistFilter f;
    check(f.process(5), true, "process(5) empty blacklist");
    check(f.block(5), true, "block(5) seen + visible -> event");
    check(f.process(5), false, "process(5) blocked");
    check(f.block(5), false, "block(5) repeat, no change");
    check(f.unblock(5), true, "unblock(5) -> visible event");
    check(f.process(5), true, "process(5) passes again");
    check(f.block(-7), false, "block(-7) never seen, silent");
    check(f.process(-7), false, "process(-7) still blocked");
    check(f.unblock(-7), true, "unblock(-7) seen while blocked -> event");
}
