// ByteDance (hack2hire) / LC 80: sorted nums; in place, keep each value at most twice.
// Return k, the new length; nums[0..k-1] holds the result in order.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Write pointer; keep x unless it equals the value two slots back. T: O(n), S: O(1).
int removeDuplicates(vector<int>& nums) {
    int w = 0;   // nums[0..w-1] = kept prefix
    for (int x : nums) {
        // sorted: nums[w-2] == x means the prefix already ends with x, x
        if (w < 2 || x != nums[w - 2]) { nums[w++] = x; }
    }
    return w;
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    int k = removeDuplicates(nums);
    nums.resize(k);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 1, 1, 2, 2, 3}, {1, 1, 2, 2, 3});
    test("lc ex2", {0, 0, 1, 1, 1, 1, 2, 3, 3}, {0, 0, 1, 1, 2, 3, 3});
    test("just two", {1, 1}, {1, 1});
}
