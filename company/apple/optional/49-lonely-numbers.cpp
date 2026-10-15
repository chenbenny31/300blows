// LC 2150: x is lonely if it appears exactly once and neither x - 1 nor x + 1 appears.
// Return all lonely numbers (any order).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// Count map, then check each number and its neighbors. T: O(n), S: O(n).
vector<int> findLonely(const vector<int>& nums) {
    unordered_map<int, int> cnt;   // cnt[x] = occurrences of x
    for (int x : nums) { cnt[x]++; }
    vector<int> result;
    for (const auto& [x, c] : cnt) {
        if (c == 1 && !cnt.count(x - 1) && !cnt.count(x + 1)) { result.push_back(x); }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    vector<int> got = findLonely(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {10, 6, 5, 8}, {8, 10});
    test("lc ex2", {1, 3, 5, 3}, {1, 5});
    test("none", {1, 1, 2}, {});
}
