#include <algorithm>
#include <cassert>
#include <climits>
#include <vector>

using std::vector;

// T: O(n), S: O(1)
bool feasible(const vector<int>& throughput, const vector<int>& cost,
              long long budget, long long target) {
    for (int i = 0; i  < static_cast<int>(throughput.size()); i++) {
        long long upgrades = (target - 1) / throughput[i];
        if (upgrades > budget / cost[i]) { return false; }
        budget -= upgrades * cost[i];
    }
    return true;
}

// T: O(nlogU), S: O(1), U = upper_bound of throughput
long long maxThroughput(const vector<int>& throughput,
                        const vector<int>& cost, int budget) {
    if (throughput.empty()) { return 0; }
    long long lo = *std::min_element(throughput.begin(), throughput.end());
    long long hi = LLONG_MAX;

    for (int i = 0; i < static_cast<int>(throughput.size()); i++) {
        long long limit = throughput[i] * (1LL + budget / cost[i]);
        hi = std::min(hi, limit);
    }

    hi++; // find upper_bound
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(throughput, cost, budget, mid)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo - 1;
}

int main() {
    // Upgrade both once: throughputs {4, 6}, total cost 3.
    assert(maxThroughput({2, 3}, {1, 2}, 3) == 4);

    assert(maxThroughput({2, 3}, {1, 2}, 0) == 2); // No upgrades.
    assert(maxThroughput({5}, {3}, 2) == 5); // Cannot afford an upgrade.

    assert(maxThroughput({1000000000}, {1}, 3) == 4000000000LL);
    // Answer needs 64 bits.

    assert(maxThroughput({}, {}, 5) == 0);
}
