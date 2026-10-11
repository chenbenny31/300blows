// query type i takes time[i] and earns profit[i]; time budget K; pick ONE type and run it as often as fits
// total = floor(K / time[i]) * profit[i]; return {best total, its index}, ties -> smallest index,
// no type fits at all -> {0, -1} (Q37)

// time positive? profit can be 0 or negative?
// only one type, repeated? (not a knapsack)
// tie on total: smallest index?
// K and totals beyond int?

#include <vector>
#include <cassert>
#include <iostream>

using std::vector;

class QueryPlanner {
public:
    // one pass, T: O(n), S: O(1)
    vector<long long> maxProfit(const vector<int>& time, const vector<int>& profit, long long K) {
        long long best = 0;
        int bestIdx = -1; // -1 = nothing fits yet; best == 0 alone can't tell "fits but earns 0"
        for (int i = 0; i < static_cast<int>(time.size()); i++) {
            if (time[i] > K) {
                continue; // can't run even once
            }
            long long total = (K / time[i]) * profit[i]; // whole runs only
            if (bestIdx == -1 || total > best) { // strict >: a tie keeps the smaller index
                best = total;
                bestIdx = i;
            }
        }
        return {best, static_cast<long long>(bestIdx)};
    }
};

int main() {
    QueryPlanner qp;
    vector<long long> want;

    want = {15, 1};
    assert(qp.maxProfit({3, 5}, {10, 15}, 5) == want); // target: better ratio 10/3 only earns 10
    want = {4, 0};
    assert(qp.maxProfit({2, 4}, {2, 4}, 4) == want); // edge: tie on total, smallest index
    want = {0, 0};
    assert(qp.maxProfit({1}, {0}, 3) == want); // edge: fits but earns 0, still a real answer
    want = {0, -1};
    assert(qp.maxProfit({6}, {9}, 5) == want); // fault: nothing fits in the budget
    assert(qp.maxProfit({}, {}, 5) == want); // null: no query types

    std::cout << "all tests passed\n";
    return 0;
}
