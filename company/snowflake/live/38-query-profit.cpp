// B14 (no direct LC): query type i takes time[i] and earns profit[i]; within budget K pick ONE type and run it
// floor(K / time[i]) times; return {max profit, index}, ties -> smallest index, nothing fits -> {0, -1}.

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

class QueryPlanner {
public:
    // one pass over types, total = (K / time) * profit, T: O(n), S: O(1)
    vector<long long> maxProfit(const vector<int>& time, const vector<int>& profit, long long K) {
        int n = time.size();
        long long best = 0;
        int bestIdx = -1;   // -1 = nothing fits yet; best == 0 can't tell "fits but earns 0"
        for (int i = 0; i < n; i++) {
            if (time[i] > K) { continue; }   // can't run even once
            long long total = (K / time[i]) * profit[i];
            // strict >: a tie keeps the earlier, smaller index
            if (bestIdx == -1 || total > best) {
                best = total;
                bestIdx = i;
            }
        }
        return {best, bestIdx};
    }
};

bool test(const string& label, const vector<int>& time, const vector<int>& profit, long long K,
          const vector<long long>& expected) {
    QueryPlanner qp;
    vector<long long> got = qp.maxProfit(time, profit, K);
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got {" << got[0] << ", " << got[1] << "}, expected {"
         << expected[0] << ", " << expected[1] << "}\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("example", {3, 5}, {10, 15}, 5, {15, 1});                  // better ratio 10/3 only fits once -> 10
    ok &= test("repeat", {2, 3}, {3, 5}, 7, {10, 1});                      // 3 runs * 3 = 9 vs 2 runs * 5 = 10
    ok &= test("empty", {}, {}, 5, {0, -1});                               // no query types
    ok &= test("none fits", {6, 9}, {9, 1}, 5, {0, -1});                   // every time > K
    ok &= test("zero K", {1}, {4}, 0, {0, -1});                            // K = 0, nothing runs
    ok &= test("fits earns 0", {7, 1}, {5, 0}, 3, {0, 1});                 // index 1 fits with profit 0, still real
    ok &= test("tie", {2, 4, 1}, {2, 4, 1}, 4, {4, 0});                    // all three total 4, smallest index
    ok &= test("exact fit", {5}, {7}, 5, {7, 0});                          // time == K runs once
    ok &= test("64-bit", {1, 2}, {1000000000, 1000000000}, 4000000000LL, {4000000000000000000LL, 0});   // total > 2^31
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
