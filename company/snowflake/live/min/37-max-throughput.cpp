// B11 · LC 875/2064 pattern: services in series, x upgrades of i (cost c[i] each) give t[i] * (x + 1); budget maximizes min throughput. Follow-up: at most cap[i] upgrades each.
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using std::cout;
using std::vector;

// binary search the last feasible target, T: O(n log U), S: O(1)
long long maxThroughput(const vector<int>& t, const vector<int>& c, long long budget) {
    int n = t.size();
    if (n == 0) { return 0; }
    auto feasible = [&](long long target) {
        long long left = budget;                    // budget not yet spent
        for (int i = 0; i < n; i++) {
            long long need = (target - 1) / t[i];   // fewest x with t[i] * (x + 1) >= target
            if (need > left / c[i]) { return false; }   // divide first, need * c[i] may overflow
            left -= need * c[i];
        }
        return true;
    };
    long long lo = *std::min_element(t.begin(), t.end());   // feasible with zero upgrades
    long long hi = LLONG_MAX;                                // service i alone can't pass t[i] * (budget / c[i] + 1)
    for (int i = 0; i < n; i++) {
        long long most = budget / c[i];
        if (most < LLONG_MAX / t[i]) { hi = std::min(hi, t[i] * (most + 1)); }   // most + 1 <= LLONG_MAX / t[i] without overflow
    }
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;     // round up, else lo = mid loops forever
        if (feasible(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

// follow-up: service i allows at most cap[i] upgrades, same search, T: O(n log U), S: O(1)
long long maxThroughputCapped(const vector<int>& t, const vector<int>& c, const vector<long long>& cap, long long budget) {
    int n = t.size();
    if (n == 0) { return 0; }
    auto feasible = [&](long long target) {
        long long left = budget;                    // budget not yet spent
        for (int i = 0; i < n; i++) {
            long long need = (target - 1) / t[i];
            if (need > cap[i] || need > left / c[i]) { return false; }
            left -= need * c[i];
        }
        return true;
    };
    long long lo = *std::min_element(t.begin(), t.end());
    long long hi = LLONG_MAX;
    for (int i = 0; i < n; i++) {
        long long most = std::min(budget / c[i], cap[i]);
        if (most < LLONG_MAX / t[i]) { hi = std::min(hi, t[i] * (most + 1)); }
    }
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;
        if (feasible(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

void test(const vector<int>& t, const vector<int>& c, long long budget, long long expect) {
    long long got = maxThroughput(t, c, budget);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testCapped(const vector<int>& t, const vector<int>& c, const vector<long long>& cap, long long budget, long long expect) {
    long long got = maxThroughputCapped(t, c, cap, budget);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test({2, 3}, {1, 2}, 3, 4);                                                     // target: both upgrade once
    test({}, {}, 5, 0);                                                             // no services
    test({1, 1}, {1, 10}, 15, 2);                                                   // expensive service caps the min
    test({1000000000, 1000000000}, {1, 1}, 2000000000LL, 1000000001000000000LL);    // answer > 2^31
    test({2000000000, 1}, {1, 1000000000}, 4000000000000000000LL, 4000000000LL);    // bound for s0 overflows, skipped

    testCapped({2, 3}, {1, 2}, {0, 5}, 3, 2);                                       // first service can't upgrade
    testCapped({2, 3}, {1, 2}, {1, 1}, 100, 4);                                     // budget plenty, caps bind
    testCapped({2, 3}, {1, 2}, {5, 5}, 3, 4);                                       // caps loose, same as uncapped
}
