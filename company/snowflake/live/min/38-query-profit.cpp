// B14 · no direct LC: type i takes time[i], earns profit[i]; within K run ONE type floor(K / time[i]) times; return {max profit, index}, tie -> smallest index, nothing fits -> {0, -1}.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// one pass over types, total = (K / time) * profit, T: O(n), S: O(1)
vector<long long> maxProfit(const vector<int>& time, const vector<int>& profit, long long K) {
    int n = time.size();
    long long best = 0;
    int bestIdx = -1;   // -1 = nothing fits yet; best == 0 can't tell "fits but earns 0"
    for (int i = 0; i < n; i++) {
        if (time[i] > K) { continue; }
        long long total = (K / time[i]) * profit[i];
        if (bestIdx == -1 || total > best) {   // strict >: a tie keeps the smaller index
            best = total;
            bestIdx = i;
        }
    }
    return {best, bestIdx};
}

void test(const string& label, const vector<int>& time, const vector<int>& profit, long long K,
          const vector<long long>& expect) {
    vector<long long> got = maxProfit(time, profit, K);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    test("example", {3, 5}, {10, 15}, 5, {15, 1});        // target: better ratio only fits once
    test("empty", {}, {}, 5, {0, -1});                    // no types
    test("fits earns 0", {7, 1}, {5, 0}, 3, {0, 1});      // feasible zero profit is still an answer
    test("tie", {2, 4, 1}, {2, 4, 1}, 4, {4, 0});         // all total 4, smallest index
    test("64-bit", {1, 2}, {1000000000, 1000000000}, 4000000000LL, {4000000000000000000LL, 0});   // total > 2^31
}
