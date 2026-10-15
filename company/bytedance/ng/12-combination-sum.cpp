// ByteDance (hack2hire) / LC 39: distinct positive candidates; return all unique
// combinations summing to target, each candidate reusable any number of times.
// Follow-up LC 40: candidates may repeat and each element is used at most once.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen candidates; remain = target - sum(path); start = first index allowed
void dfs(const vector<int>& nums, int start, int remain, vector<int>& path,
         vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    for (int i = start; i < (int)nums.size(); i++) {
        // sorted: every later candidate is too big as well
        if (nums[i] > remain) { break; }
        path.push_back(nums[i]);
        dfs(nums, i, remain - nums[i], path, result);   // i, not i + 1: reuse allowed
        path.pop_back();
    }
}

// Sort, then backtrack with pruning. T: O(n^(t/m)), S: O(t/m);
// t = target, m = smallest candidate (max depth t/m).
vector<vector<int>> combinationSum(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> result;
    vector<int> path;
    dfs(candidates, 0, target, path, result);
    return result;
}

// follow-up: recurse with i + 1; skip a value equal to the previous sibling
void dfsOnce(const vector<int>& nums, int start, int remain, vector<int>& path,
             vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    for (int i = start; i < (int)nums.size(); i++) {
        if (nums[i] > remain) { break; }
        // same value at the same depth would repeat a combination
        if (i > start && nums[i] == nums[i - 1]) { continue; }
        path.push_back(nums[i]);
        dfsOnce(nums, i + 1, remain - nums[i], path, result);
        path.pop_back();
    }
}

// T: O(2^n * n), S: O(n).
vector<vector<int>> combinationSum2(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> result;
    vector<int> path;
    dfsOnce(candidates, 0, target, path, result);
    return result;
}

void test(const string& label, const vector<int>& candidates, int target,
          const vector<vector<int>>& want) {
    cout << label << (combinationSum(candidates, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 3, 6, 7}, 7, {{2, 2, 3}, {7}});
    test("lc ex2", {2, 3, 5}, 8, {{2, 2, 2, 2}, {2, 3, 3}, {3, 5}});
    test("none", {2}, 1, {});
}
