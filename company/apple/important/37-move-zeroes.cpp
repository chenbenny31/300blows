// LC 283: move all 0s in nums to the end in place, keeping the order of the non-zeros.
#include <iostream>
#include <vector>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::swap;

// Write pointer: swap each non-zero into the next free slot. T: O(n), S: O(1).
void moveZeroes(vector<int>& nums) {
    int write = 0;   // nums[0..write) = non-zeros seen so far, in order
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (nums[i] != 0) { swap(nums[write++], nums[i]); }
    }
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    moveZeroes(nums);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {0, 1, 0, 3, 12}, {1, 3, 12, 0, 0});
    test("only zero", {0}, {0});
    test("no zeros", {1, 2}, {1, 2});
}
