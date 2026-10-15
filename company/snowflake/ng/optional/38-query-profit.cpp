// B14 (no direct LC): query type i takes time[i] and earns profit[i]. Pick ONE type and
// run it floor(K / time[i]) times; return {max profit, index}, ties -> smallest index,
// nothing fits -> {0, -1}.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

// One pass: a chosen type is best run as often as it fits. T: O(n), S: O(1).
pair<long long, int> maxProfit(const vector<int>& time, const vector<int>& profit,
                               long long K) {
    int n = time.size();
    long long best = 0;   // best total so far
    int bestIdx = -1;     // -1 = nothing fits yet (a type that fits may still earn 0)
    for (int i = 0; i < n; i++) {
        if (time[i] > K) { continue; }
        long long total = K / time[i] * profit[i];
        // strict >: a tie keeps the earlier, smaller index
        if (bestIdx == -1 || total > best) {
            best = total;
            bestIdx = i;
        }
    }
    return {best, bestIdx};
}

void test(const string& label, const vector<int>& time, const vector<int>& profit,
          long long K, pair<long long, int> want) {
    cout << label << (maxProfit(time, profit, K) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("ratio trap", {3, 5}, {10, 15}, 5, {15, 1});
    test("tie", {2, 4, 1}, {2, 4, 1}, 4, {4, 0});
    test("none fits", {6, 9}, {9, 1}, 5, {0, -1});
    test("fits earns 0", {7, 1}, {5, 0}, 3, {0, 1});
}
