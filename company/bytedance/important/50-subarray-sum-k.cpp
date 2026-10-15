// ByteDance / LC 560: count the contiguous non-empty subarrays of nums (values may be
// negative) whose sum equals k.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Prefix sums: a subarray ending here sums to k iff an earlier prefix == sum - k.
// T: O(n), S: O(n).
int subarraySum(const vector<int>& nums, int k) {
    // seen[p] = earlier prefixes with sum p; the empty prefix counts once
    unordered_map<int, int> seen{{0, 1}};
    int sum = 0, count = 0;
    for (int x : nums) {
        sum += x;
        auto it = seen.find(sum - k);
        if (it != seen.end()) { count += it->second; }
        seen[sum]++;   // after the lookup, so k == 0 never counts an empty subarray
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 1, 1}, 2, 2);
    test("lc ex2", {1, 2, 3}, 3, 2);
    test("negative", {1, -1, 0}, 0, 3);
}
