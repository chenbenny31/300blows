// LC 15: return all unique triplets [a, b, c] from nums with a + b + c == 0.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort, fix the smallest value, two pointers on the rest. T: O(n^2), S: O(1) + output.
vector<vector<int>> threeSum(vector<int> nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> result;   // triplets in ascending order
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        // same first value would give the same triplets
        if (i > 0 && nums[i] == nums[i - 1]) { continue; }
        int l = i + 1, r = n - 1;
        while (l < r) {
            int sum = nums[i] + nums[l] + nums[r];
            if (sum < 0) {
                l++;
            } else if (sum > 0) {
                r--;
            } else {
                result.push_back({nums[i], nums[l], nums[r]});
                l++;
                r--;
                // skip equal values so the same pair is not added again
                while (l < r && nums[l] == nums[l - 1]) { l++; }
            }
        }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<vector<int>>& want) {
    cout << label << (threeSum(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {-1, 0, 1, 2, -1, -4}, {{-1, -1, 2}, {-1, 0, 1}});
    test("all zeros", {0, 0, 0, 0}, {{0, 0, 0}});
    test("none", {0, 1, 1}, {});
}
