// LC 448 Find All Numbers Disappeared in an Array: nums has n values in [1, n]; return
// every value in [1, n] that does not appear, in O(n) time and O(1) extra space.
// Trick: sign marking; value v negates nums[v-1]; slots still positive are missing
// Recognize: values in 1..n + O(1) space -> the sign of nums[v-1] is a "seen v" flag.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::abs;

// Sign marking; abs() still reads a value whose slot was already negated.
// T: O(n), S: O(1) besides the output.
vector<int> findDisappearedNumbers(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        int v = abs(nums[i]);
        // negate only once, so a repeated v keeps the slot negative
        if (nums[v - 1] > 0) { nums[v - 1] = -nums[v - 1]; }
    }
    vector<int> missing;   // values never seen, ascending
    for (int i = 0; i < n; i++) {
        if (nums[i] > 0) { missing.push_back(i + 1); }
    }
    return missing;
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    cout << label << (findDisappearedNumbers(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {4, 3, 2, 7, 8, 2, 3, 1}, {5, 6});
    test("duplicate", {1, 1}, {2});
}
