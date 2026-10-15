// LC 189: rotate nums to the right by k steps in place (k may exceed n).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::reverse;

// Reverse all, then reverse the first k and the rest. T: O(n), S: O(1).
void rotate(vector<int>& nums, int k) {
    int n = nums.size();
    if (n == 0) { return; }
    k %= n;   // rotating by n is a no-op
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}

void test(const string& label, vector<int> nums, int k, const vector<int>& want) {
    rotate(nums, k);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 3, 4, 5, 6, 7}, 3, {5, 6, 7, 1, 2, 3, 4});
    test("k > n", {1, 2}, 3, {2, 1});
    test("k = 0", {1, 2, 3}, 0, {1, 2, 3});
}
