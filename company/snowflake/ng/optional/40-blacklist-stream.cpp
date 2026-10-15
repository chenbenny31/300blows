// B19 (no direct LC; 706 set basics): filter an integer stream against a blacklist that
// changes at runtime; process(x) returns whether x passes.
#include <iostream>
#include <string>
#include <unordered_set>
using std::cout;
using std::string;
using std::unordered_set;

class BlacklistFilter {
    unordered_set<int> blocked;   // values currently on the blacklist

public:
    // Hash set lookup. T: O(1) average per call, S: O(B), B = blacklist size.
    // Every occurrence is judged on its own: repeats pass each time.
    bool process(int x) { return blocked.count(x) == 0; }

    // follow-up: add a `seen` set; block/unblock return true when a seen x flips.
    void block(int x) { blocked.insert(x); }

    void unblock(int x) { blocked.erase(x); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    BlacklistFilter f;
    check(f.process(42), "empty blacklist");
    f.block(42);
    check(!f.process(42) && f.process(7), "blocked");
    f.block(42);
    f.unblock(42);
    check(f.process(42) && f.process(42), "unblock after double block");
}
