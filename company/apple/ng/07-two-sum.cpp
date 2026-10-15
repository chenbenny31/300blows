// LC 1: return indices of the two numbers in nums that add up to target.
// Exactly one solution exists; the same element can't be used twice.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// One pass with a value -> index map. T: O(n), S: O(n).
vector<int> twoSum(const vector<int>& nums, int target) {
    unordered_map<int, int> seen;   // seen[value] = index of an earlier element
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // look up before insert, so nums[i] never pairs with itself
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) { return {it->second, i}; }
        seen[nums[i]] = i;
    }
    return {};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (twoSum(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 7, 11, 15}, 9, {0, 1});
    test("not self", {3, 2, 4}, 6, {1, 2});
    test("duplicates", {3, 3}, 6, {0, 1});
}
