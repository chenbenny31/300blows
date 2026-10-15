// ByteDance (hack2hire) / LC 163: sorted nums; list the numbers of [lower, upper] that
// are missing from nums: a single number as {a}, a run as {a, b}. Ignore nums outside.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

void addRange(vector<vector<int>>& result, long long a, long long b) {
    if (a == b) {
        result.push_back({(int)a});
    } else {
        result.push_back({(int)a, (int)b});
    }
}

// Scan with the next number not yet covered. T: O(n), S: O(1) besides the output.
vector<vector<int>> findMissingRanges(const vector<int>& nums, int lower, int upper) {
    vector<vector<int>> result;
    // smallest number not yet covered; long long so x + 1 cannot overflow
    long long next = lower;
    for (int x : nums) {
        // below lower or a duplicate
        if (x < next) { continue; }
        if (x > upper) { break; }
        if (x > next) { addRange(result, next, x - 1); }
        next = (long long)x + 1;
    }
    if (next <= upper) { addRange(result, next, upper); }
    return result;
}

void test(const string& label, const vector<int>& nums, int lower, int upper,
          const vector<vector<int>>& want) {
    bool ok = findMissingRanges(nums, lower, upper) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {3, 5, 10}, 0, 9, {{0, 2}, {4}, {6, 9}});
    test("singles", {1, 3, 5}, 0, 7, {{0}, {2}, {4}, {6, 7}});
    test("empty", {}, 0, 5, {{0, 5}});
}
