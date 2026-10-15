// B11 (no direct LC; 875 / 2064 pattern): services in series, x upgrades of service i (cost c[i] each)
// give throughput t[i] * (x + 1); spend at most budget to maximize min throughput over all services.

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

class ThroughputPlanner {
public:
    // binary search the last feasible target, T: O(n log U), U: upper bound on the answer, S: O(1)
    long long maxThroughput(const vector<int>& t, const vector<int>& c, long long budget) {
        int n = t.size();
        if (n == 0) { return 0; }
        long long lo = *std::min_element(t.begin(), t.end());   // feasible with zero upgrades
        long long hi = LLONG_MAX;                                // service i alone can't pass t[i] * (budget / c[i] + 1)
        for (int i = 0; i < n; i++) {
            long long most = budget / c[i];
            // most + 1 <= LLONG_MAX / t[i], written so neither side overflows
            if (most < LLONG_MAX / t[i]) { hi = std::min(hi, t[i] * (most + 1)); }
        }
        while (lo < hi) {
            long long mid = lo + (hi - lo + 1) / 2;   // round up, else lo = mid loops forever
            if (feasible(t, c, budget, mid)) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    }

    // follow-up: service i allows at most cap[i] upgrades; same search, T: O(n log U), S: O(1)
    long long maxThroughputCapped(const vector<int>& t, const vector<int>& c, const vector<long long>& cap, long long budget) {
        int n = t.size();
        if (n == 0) { return 0; }
        long long lo = *std::min_element(t.begin(), t.end());   // feasible with zero upgrades
        long long hi = LLONG_MAX;                                // service i alone can't pass t[i] * (most + 1)
        for (int i = 0; i < n; i++) {
            long long most = std::min(budget / c[i], cap[i]);
            if (most < LLONG_MAX / t[i]) { hi = std::min(hi, t[i] * (most + 1)); }
        }
        while (lo < hi) {
            long long mid = lo + (hi - lo + 1) / 2;
            if (feasibleCapped(t, c, cap, budget, mid)) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    }

private:
    // can every service reach target within budget?
    bool feasible(const vector<int>& t, const vector<int>& c, long long budget, long long target) {
        int n = t.size();
        long long left = budget;   // budget not yet spent
        for (int i = 0; i < n; i++) {
            long long need = (target - 1) / t[i];   // fewest x with t[i] * (x + 1) >= target
            // compare by division so need * c[i] is only computed when it fits in left
            if (need > left / c[i]) { return false; }
            left -= need * c[i];
        }
        return true;
    }

    bool feasibleCapped(const vector<int>& t, const vector<int>& c, const vector<long long>& cap, long long budget, long long target) {
        int n = t.size();
        long long left = budget;   // budget not yet spent
        for (int i = 0; i < n; i++) {
            long long need = (target - 1) / t[i];
            if (need > cap[i] || need > left / c[i]) { return false; }
            left -= need * c[i];
        }
        return true;
    }
};

bool test(const string& label, const vector<int>& t, const vector<int>& c, long long budget, long long expected) {
    ThroughputPlanner tp;
    long long got = tp.maxThroughput(t, c, budget);
    long long got2 = tp.maxThroughputCapped(t, c, vector<long long>(t.size(), LLONG_MAX), budget);   // no real cap
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << " / " << got2 << ", expected " << expected << "\n";
    return false;
}

bool testCapped(const string& label, const vector<int>& t, const vector<int>& c, const vector<long long>& cap,
                long long budget, long long expected) {
    ThroughputPlanner tp;
    long long got = tp.maxThroughputCapped(t, c, cap, budget);
    if (got == expected) { return true; }
    cout << "FAIL capped " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("example", {2, 3}, {1, 2}, 3, 4);                     // both upgrade once (cost 3); 5 needs cost 4
    ok &= test("no budget", {2, 3}, {1, 2}, 0, 2);                   // slowest service decides
    ok &= test("empty", {}, {}, 5, 0);                               // no services
    ok &= test("single short", {5}, {3}, 2, 5);                      // one service, can't afford one upgrade
    ok &= test("single", {5}, {3}, 6, 15);                           // two upgrades -> 5 * 3
    ok &= test("shared budget", {1, 1}, {1, 10}, 15, 2);             // expensive service caps the min, not the cheap one
    ok &= test("64-bit", {1000000000, 1000000000}, {1, 1}, 2000000000LL, 1000000001000000000LL);   // answer > 2^31
    ok &= test("hi overflow", {2000000000, 1}, {1, 1000000000}, 4000000000000000000LL, 4000000000LL); // s0 bound overflows, skipped
    ok &= testCapped("cap 0", {2, 3}, {1, 2}, {0, 5}, 3, 2);          // first service can't upgrade at all
    ok &= testCapped("cap binds", {2, 3}, {1, 2}, {1, 1}, 100, 4);    // budget plenty, caps give min(4, 6)
    ok &= testCapped("cap loose", {2, 3}, {1, 2}, {5, 5}, 3, 4);      // caps not reached, same as uncapped
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
