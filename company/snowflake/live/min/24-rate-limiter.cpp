// B15 · LC 359 / 362 prototypes: per-client sliding-window log, at most limit accepted in (t - W, t]. Follow-up: several (W, limit) rules, all must pass.
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::unordered_map;

class SlidingWindowLimiter {
public:
    SlidingWindowLimiter(int limit, long long windowMs) : limit(limit), windowMs(windowMs) {}

    // sliding window log per client, T: O(1) amortized, S: O(C*limit), C = clients
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

    // follow-up: check every rule first, consume only if all pass, T: O(R) amortized, S: O(sum of limits)
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
    vector<long long> windows;       // rule -> window length W
    vector<int> limits;              // rule -> max accepted per window
    vector<deque<long long>> logs;   // rule -> accepted timestamps in its window, oldest first
};

void check(bool got, bool expect, const string& label) {
    cout << (got == expect ? "PASS " : "FAIL ") << label << " " << got << " expect " << expect << "\n";
}

int main() {
    SlidingWindowLimiter rl(3, 1000);
    check(rl.allow("a", 0), true, "a@0");
    check(rl.allow("a", 100), true, "a@100");
    check(rl.allow("a", 200), true, "a@200");
    check(rl.allow("a", 300), false, "a@300 4th in window");
    check(rl.allow("b", 300), true, "b@300 separate client");
    check(rl.allow("a", 1000), true, "a@1000 drops 0 at exact t - W, rejected 300 not counted");
    check(rl.allow("a", 1001), false, "a@1001 holds 100, 200, 1000");
    SlidingWindowLimiter zero(0, 1000);
    check(zero.allow("a", 0), false, "limit 0 rejects");

    MultiRuleLimiter ml;
    ml.registerRule(1000, 2);
    ml.registerRule(10, 1);
    check(ml.allowRequest(0), true, "@0 both empty");
    check(ml.allowRequest(5), false, "@5 short rule full");
    check(ml.allowRequest(20), true, "@20 long rule not charged by rejected @5");
    check(ml.allowRequest(995), false, "@995 long rule full");
    check(ml.allowRequest(1000), true, "@1000 short rule not charged by rejected @995");
    MultiRuleLimiter none;
    check(none.allowRequest(0), true, "no rules allows");
}
