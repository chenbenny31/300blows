// LC 31 Next Permutation: rearrange nums in place into the next lexicographic
// permutation; if it is the largest, wrap to the smallest (sorted ascending).
// Trick: rightmost dip a[i] < a[i+1]; swap a[i] with rightmost bigger; reverse suffix
// Recognize: "next greater arrangement" of a sequence, in place.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::reverse;

// The suffix after the dip is non-increasing (its largest arrangement), so bump a[i]
// by the least amount and make the suffix its smallest arrangement. T: O(n), S: O(1).
void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) { i--; }
    if (i >= 0) {
        int j = n - 1;
        // rightmost element > nums[i] = the smallest one bigger than it in the suffix
        while (nums[j] <= nums[i]) { j--; }
        swap(nums[i], nums[j]);
    }
    // suffix is still non-increasing after the swap; i = -1 reverses the whole array
    reverse(nums.begin() + i + 1, nums.end());
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    nextPermutation(nums);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 3}, {1, 3, 2});
    test("wrap around", {3, 2, 1}, {1, 2, 3});
    test("duplicates", {1, 1, 5}, {1, 5, 1});
    test("reverse suffix", {1, 3, 2}, {2, 1, 3});
}
