// B11 (no direct LC; 875 / 2064 pattern): services in series; x upgrades of service i
// (cost c[i] each) give throughput t[i] * (x + 1). Spend at most budget to maximize
// the minimum throughput over all services.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::string;
using std::vector;
using std::min_element;

// Can every service reach target within budget? Greedy per service, T: O(n).
bool feasible(const vector<int>& t, const vector<int>& c, long long budget,
              long long target) {
    int n = t.size();
    long long left = budget;   // budget not yet spent
    for (int i = 0; i < n; i++) {
        long long need = (target - 1) / t[i];   // fewest x with t[i] * (x + 1) >= target
        // stop once cost > budget; divide instead of need * c[i], which can overflow
        if (need > left / c[i]) { return false; }
        left -= need * c[i];
    }
    return true;
}

// Binary search the largest feasible target (feasible is monotone in target).
// T: O(n log U), S: O(1); U = upper bound on the answer, log U <= 63.
long long maxThroughput(const vector<int>& t, const vector<int>& c, long long budget) {
    if (t.empty()) { return 0; }
    long long lo = *min_element(t.begin(), t.end());   // reached with zero upgrades
    // service 0 gets at most budget / c[0] upgrades; guard the multiply
    long long most = budget / c[0];
    long long hi = most < LLONG_MAX / t[0] ? t[0] * (most + 1) : LLONG_MAX;
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

void test(const string& label, const vector<int>& t, const vector<int>& c,
          long long budget, long long want) {
    cout << label << (maxThroughput(t, c, budget) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {2, 3}, {1, 2}, 3, 4);
    test("no budget", {2, 3}, {1, 2}, 0, 2);
    test("shared budget", {1, 1}, {1, 10}, 15, 2);
    test("64-bit", {1000000000, 1000000000}, {1, 1}, 2000000000LL, 1000000001000000000LL);
}
