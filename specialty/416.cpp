// LC 416 Partition Equal Subset Sum: can nums (positive, sum <= 20000) be split into two
// subsets with equal sums?
// Trick: odd total -> false; else 0/1 subset-sum to total/2, sums iterated backward
// Recognize: "split into two equal halves" = does some subset reach exactly half
// Follow-up: bitset<20001> where reach |= reach << x adds x to every reachable sum.
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
using std::cout;
using std::string;
using std::vector;
using std::bitset;

// 1D 0/1 knapsack on reachable sums. T: O(n * total), S: O(total).
bool canPartition(const vector<int>& nums) {
    int total = 0;
    for (int x : nums) { total += x; }
    if (total % 2 == 1) { return false; }
    int half = total / 2;
    vector<bool> reach(half + 1, false);   // reach[s] = some subset so far sums to s
    reach[0] = true;
    for (int x : nums) {
        // backward so reach[s - x] still excludes x: each number is used at most once
        for (int s = half; s >= x; s--) {
            if (reach[s - x]) { reach[s] = true; }
        }
    }
    return reach[half];
}

// follow-up: bit s set = sum s reachable; one shift-or per number.
// T: O(n * 20000 / 64), S: O(20000).
bool canPartitionBits(const vector<int>& nums) {
    int total = 0;
    for (int x : nums) { total += x; }
    if (total % 2 == 1) { return false; }
    bitset<20001> reach;   // reach[s] = some subset so far sums to s
    reach[0] = 1;
    for (int x : nums) { reach |= reach << x; }
    return reach[total / 2];
}

void test(const string& label, const vector<int>& nums, bool want) {
    cout << label << (canPartition(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 5, 11, 5}, true);
    test("lc ex2", {1, 2, 3, 5}, false);
    test("pair", {2, 2}, true);
    test("even unreachable", {1, 2, 5}, false);
}
