// LC 75 Sort Colors: sort an array of 0s, 1s and 2s in place in one pass.
// Trick: Dutch flag lo/i/hi; after swapping with hi, don't advance i (unchecked value)
// Recognize: 3-way partition around a pivot value in one pass.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::swap;

// Dutch national flag: [0, lo) = 0s, [lo, i) = 1s, (hi, n-1] = 2s, [i, hi] unknown.
// T: O(n), S: O(1).
void sortColors(vector<int>& nums) {
    int lo = 0, i = 0, hi = (int)nums.size() - 1;
    while (i <= hi) {
        if (nums[i] == 0) {
            // nums[lo] is a known 1 (or i itself), so i can advance
            swap(nums[lo++], nums[i++]);
        } else if (nums[i] == 2) {
            // the value from hi is unseen: re-check it, so i stays
            swap(nums[i], nums[hi--]);
        } else {
            i++;
        }
    }
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    sortColors(nums);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 0, 2, 1, 1, 0}, {0, 0, 1, 1, 2, 2});
    test("lc ex2", {2, 0, 1}, {0, 1, 2});
    test("single", {0}, {0});
}
