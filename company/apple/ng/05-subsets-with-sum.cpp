// Apple (hack2hire): all subsets of arr (each element used at most once) whose sum is
// exactly target, via backtracking.
// Follow-up: arr has duplicates and each distinct subset should appear once.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen elements so far, sum = their total
void dfs(const vector<int>& arr, int target, int start, int sum, vector<int>& path,
         vector<vector<int>>& result) {
    if (sum == target) { result.push_back(path); }
    for (int i = start; i < (int)arr.size(); i++) {
        path.push_back(arr[i]);
        dfs(arr, target, i + 1, sum + arr[i], path, result);
        path.pop_back();
    }
}

// every subset is one node of the include/exclude tree: 2^n nodes, copying a hit is O(n).
// T: O(2^n * n), S: O(n) recursion + path, plus the output (up to 2^n subsets).
vector<vector<int>> subsetsWithSum(const vector<int>& arr, int target) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(arr, target, 0, 0, path, result);
    return result;
}

// follow-up: sort, then at one depth skip a value equal to the previous sibling
void dfsNoDup(const vector<int>& arr, int target, int start, int sum, vector<int>& path,
              vector<vector<int>>& result) {
    if (sum == target) { result.push_back(path); }
    for (int i = start; i < (int)arr.size(); i++) {
        if (i > start && arr[i] == arr[i - 1]) { continue; }
        path.push_back(arr[i]);
        dfsNoDup(arr, target, i + 1, sum + arr[i], path, result);
        path.pop_back();
    }
}

vector<vector<int>> subsetsWithSumNoDup(vector<int> arr, int target) {
    sort(arr.begin(), arr.end());
    vector<vector<int>> result;
    vector<int> path;
    dfsNoDup(arr, target, 0, 0, path, result);
    return result;
}

void test(const string& label, const vector<int>& arr, int target,
          const vector<vector<int>>& want) {
    cout << label << (subsetsWithSum(arr, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {2, 3, 5, 6, 8, 10}, 10, {{2, 3, 5}, {2, 8}, {10}});
    test("none", {4, 6}, 3, {});
    test("target 0", {1, 2}, 0, {{}});
}
