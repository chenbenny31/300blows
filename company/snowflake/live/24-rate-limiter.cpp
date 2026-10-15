// B15 (LC 359 / 362 prototypes): per-client sliding-window log, at most `limit` accepted requests in (t - W, t];
// second class: several (windowMs, limit) rules, a request is accepted only if ALL rules pass, then consumes all.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>
#include <deque>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::deque;

class SlidingWindowLimiter {
public:
    SlidingWindowLimiter(int limit, long long windowMs) : limit(limit), windowMs(windowMs) {}

    // sliding window log per client, T: O(1) amortized, S: O(C*limit), C: clients
    bool allow(const string& clientId, long long timestampMs) {
        deque<long long>& log = accepted[clientId];
        while (!log.empty() && log.front() <= timestampMs - windowMs) { log.pop_front(); }   // keep (t - W, t]
        if ((int)log.size() >= limit) { return false; }   // rejected requests are not logged
        log.push_back(timestampMs);
        return true;
    }

private:
    int limit;                                          // max accepted per window
    long long windowMs;                                 // window length W
    unordered_map<string, deque<long long>> accepted;   // client -> accepted timestamps in window, oldest first
};

class MultiRuleLimiter {
public:
    void registerRule(long long windowMs, int limit) {
        windows.push_back(windowMs);
        limits.push_back(limit);
        logs.push_back({});   // a new rule only sees requests accepted after it was registered
    }

    // check every rule first, consume only if all pass, T: O(R) amortized, S: O(sum of limits), R: rules
    bool allowRequest(long long timestampMs) {
        int r = logs.size();
        for (int i = 0; i < r; i++) {
            while (!logs[i].empty() && logs[i].front() <= timestampMs - windows[i]) { logs[i].pop_front(); }
            if ((int)logs[i].size() >= limits[i]) { return false; }   // nothing pushed yet, so no rule is charged
        }
        for (int i = 0; i < r; i++) { logs[i].push_back(timestampMs); }
        return true;
    }

private:
    vector<long long> windows;        // rule -> window length W
    vector<int> limits;               // rule -> max accepted per window
    vector<deque<long long>> logs;    // rule -> accepted timestamps in its window, oldest first
};

bool check(bool got, bool expected, const string& label) {
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool testSlidingWindow() {
    bool ok = true;
    SlidingWindowLimiter rl(3, 1000);
    ok &= check(rl.allow("a", 0), true, "a@0");
    ok &= check(rl.allow("a", 100), true, "a@100");
    ok &= check(rl.allow("a", 200), true, "a@200");
    ok &= check(rl.allow("a", 300), false, "a@300 4th in window");
    ok &= check(rl.allow("b", 300), true, "b@300 separate client");
    ok &= check(rl.allow("a", 999), false, "a@999 window (-1,999] still full");
    ok &= check(rl.allow("a", 1000), true, "a@1000 drops 0, rejected 300/999 not counted");
    ok &= check(rl.allow("a", 1001), false, "a@1001 holds 100,200,1000");
    ok &= check(rl.allow("a", 1100), true, "a@1100 drops 100 at exact t-W");
    return ok;
}

bool testSmallLimits() {
    bool ok = true;
    SlidingWindowLimiter zero(0, 1000);
    ok &= check(zero.allow("a", 0), false, "limit 0 rejects");
    SlidingWindowLimiter one(1, 1);
    ok &= check(one.allow("a", 5), true, "limit1 w1 @5");
    ok &= check(one.allow("a", 5), false, "limit1 w1 same ms");
    ok &= check(one.allow("a", 6), true, "limit1 w1 @6 window (5,6]");
    return ok;
}

bool testMultiRule() {
    bool ok = true;
    MultiRuleLimiter ml;
    ml.registerRule(1000, 2);   // rule A
    ml.registerRule(10, 1);     // rule B
    ok &= check(ml.allowRequest(0), true, "@0 both empty");
    ok &= check(ml.allowRequest(5), false, "@5 B full");
    ok &= check(ml.allowRequest(20), true, "@20 A not charged by rejected @5");
    ok &= check(ml.allowRequest(25), false, "@25 both full");
    ok &= check(ml.allowRequest(995), false, "@995 A full, B passes");
    ok &= check(ml.allowRequest(1000), true, "@1000 B not charged by rejected @995");
    ok &= check(ml.allowRequest(1000), false, "@1000 again B full");
    return ok;
}

bool testNoRulesAndLateRule() {
    bool ok = true;
    MultiRuleLimiter none;
    ok &= check(none.allowRequest(0), true, "no rules allows");
    ok &= check(none.allowRequest(0), true, "no rules allows again");
    MultiRuleLimiter ml;
    ml.registerRule(100, 1);
    ok &= check(ml.allowRequest(0), true, "late @0");
    ml.registerRule(1000, 1);   // starts empty, does not see @0
    ok &= check(ml.allowRequest(200), true, "late @200 new rule empty");
    ok &= check(ml.allowRequest(300), false, "late @300 new rule holds 200");
    return ok;
}

int main() {
    bool ok = true;
    ok &= testSlidingWindow();      // client isolation, open left edge, rejections not logged
    ok &= testSmallLimits();        // smallest: limit 0, limit 1 with 1 ms window
    ok &= testMultiRule();          // adversarial: a rejection must not charge any rule
    ok &= testNoRulesAndLateRule(); // no rules, rule registered mid-stream
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
