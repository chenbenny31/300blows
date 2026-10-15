// ByteDance (hack2hire) / LC 78: all subsets of distinct nums (the power set).
// Follow-up LC 90: nums may contain duplicates; each distinct subset appears once.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen elements so far; every node of the recursion tree is a subset
void dfs(const vector<int>& nums, int start, vector<int>& path,
         vector<vector<int>>& result) {
    result.push_back(path);
    for (int i = start; i < (int)nums.size(); i++) {
        path.push_back(nums[i]);
        dfs(nums, i + 1, path, result);
        path.pop_back();
    }
}

// Backtracking: 2^n subsets, copying each is O(n).
// T: O(2^n * n), S: O(n) recursion + path, plus the output.
vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(nums, 0, path, result);
    return result;
}

// follow-up: sort, then at one depth skip a value equal to the previous sibling
void dfsDup(const vector<int>& nums, int start, vector<int>& path,
            vector<vector<int>>& result) {
    result.push_back(path);
    for (int i = start; i < (int)nums.size(); i++) {
        if (i > start && nums[i] == nums[i - 1]) { continue; }
        path.push_back(nums[i]);
        dfsDup(nums, i + 1, path, result);
        path.pop_back();
    }
}

// T: O(2^n * n), S: O(n) plus the output.
vector<vector<int>> subsetsWithDup(vector<int> nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> result;
    vector<int> path;
    dfsDup(nums, 0, path, result);
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<vector<int>>& want) {
    cout << label << (subsets(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 3}, {{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}});
    test("single", {0}, {{}, {0}});
}
