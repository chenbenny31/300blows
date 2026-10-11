// n services in series, service i has base throughput t[i], one upgrade costs c[i]
// after x upgrades its throughput is t[i] * (x + 1); total budget B
// maximize the system throughput = min over services (Q34)

// t, c positive integers? B >= 0?
// upgrades unlimited, any count per service?
// throughput t * (x + 1), linear in upgrades?
// return the max throughput only, or the upgrade plan?
// value bounds: does t * (1 + B / c) fit in 64 bits?

#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <iostream>

using std::vector;

class ThroughputPlanner {
public:
    // binary search the last feasible target T; cost to reach T is per service, so feasible(T) is monotone
    // T: O(n log U), U: hi - lo, S: O(1)
    long long maxThroughput(const vector<long long>& t, const vector<long long>& c, long long budget) {
        if (t.empty()) {
            return 0;
        }
        long long lo = *std::min_element(t.begin(), t.end()); // reachable with zero upgrades
        long long hi = LLONG_MAX; // tightest per-service bound: all budget on one service
        for (int i = 0; i < static_cast<int>(t.size()); i++) {
            long long maxUpgrades = budget / c[i];
            if (maxUpgrades + 1 <= LLONG_MAX / t[i]) { // check by division before multiplying
                hi = std::min(hi, t[i] * (maxUpgrades + 1));
            }
        }

        while (lo < hi) {
            long long mid = lo + (hi - lo + 1) / 2; // round up, else lo = mid loops forever
            if (feasible(t, c, budget, mid)) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        return lo;
    }

private:
    // can every service reach `target` within budget?
    bool feasible(const vector<long long>& t, const vector<long long>& c, long long budget, long long target) {
        long long cost = 0;
        for (int i = 0; i < static_cast<int>(t.size()); i++) {
            long long need = (target - 1) / t[i]; // = ceil(target / t) - 1 for target >= 1
            if (need > (budget - cost) / c[i]) {
                return false; // over budget; checked by division so cost never overflows
            }
            cost += need * c[i];
        }
        return true;
    }
};

int main() {
    ThroughputPlanner tp;

    assert(tp.maxThroughput({2, 3}, {1, 2}, 3) == 4); // target: upgrade both once, cost 3; 5 needs cost 4
    assert(tp.maxThroughput({2, 3}, {1, 2}, 0) == 2); // edge: no budget, slowest service decides
    assert(tp.maxThroughput({1000000000, 1000000000}, {1, 1}, 2000000000) == 1000000001000000000LL); // fault: needs 64-bit
    assert(tp.maxThroughput({}, {}, 5) == 0); // null: no services

    std::cout << "all tests passed\n";
    return 0;
}
