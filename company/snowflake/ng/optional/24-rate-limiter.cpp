// B15 · LC 359 variant: per client, accept at most `limit` requests in (t - W, t];
// rejected requests are not counted.
// Follow-up: several (windowMs, limit) rules; accept only if ALL pass, then charge all.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <deque>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::deque;

class RateLimiter {
    int limit;            // max accepted per window
    long long windowMs;   // window length W
    // accepted[client] = that client's accepted times in the window, oldest first
    unordered_map<string, deque<long long>> accepted;

public:
    RateLimiter(int lim, long long w) : limit(lim), windowMs(w) {}

    // Sliding window log: drop expired times, then accept if there is room.
    // T: O(1) amortized, S: O(C * limit) for C clients.
    bool allow(const string& clientId, long long timestampMs) {
        deque<long long>& times = accepted[clientId];
        // keep only (t - W, t]: a time exactly W ago has expired
        while (!times.empty() && times.front() <= timestampMs - windowMs) {
            times.pop_front();
        }
        if ((int)times.size() >= limit) { return false; }
        times.push_back(timestampMs);
        return true;
    }
};

// follow-up: global rules; check every rule before charging any.
// T: O(R) amortized per request, R = rules.
class MultiRuleLimiter {
    struct Rule {
        long long windowMs;
        int limit;
        deque<long long> times;   // accepted times in this rule's window, oldest first
    };
    vector<Rule> rules;

public:
    // a new rule starts empty: it only sees requests accepted after registration
    void registerRule(long long windowMs, int limit) {
        rules.push_back({windowMs, limit, {}});
    }

    bool allowRequest(long long t) {
        for (Rule& r : rules) {
            while (!r.times.empty() && r.times.front() <= t - r.windowMs) {
                r.times.pop_front();
            }
            // nothing is charged yet, so a reject leaves every rule untouched
            if ((int)r.times.size() >= r.limit) { return false; }
        }
        for (Rule& r : rules) { r.times.push_back(t); }
        return true;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RateLimiter rl(2, 1000);
    check(rl.allow("alice", 0) && rl.allow("alice", 500), "under limit");
    check(!rl.allow("alice", 999), "over limit");
    check(rl.allow("bob", 999), "per client");
    check(rl.allow("alice", 1000), "window edge");
}
