// LC 41 First Missing Positive: return the smallest positive integer missing from the
// unsorted array nums, in O(n) time and O(1) extra space.
// Trick: cyclic sort; index as hash: put value v in slot v-1, then find the first misfit
// Recognize: O(1) space and only values 1..n matter -> use the array itself as the hash.
#include <iostream>
#include <vector>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::swap;

// Cyclic sort: answer is in 1..n+1, so only in-range values need a home slot.
// T: O(n), S: O(1).
int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // each swap puts one value home for good: at most n swaps in total;
        // the home-slot check also stops a duplicate from swapping forever
        while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) { return i + 1; }
    }
    return n + 1;
}

void test(const string& label, vector<int> nums, int want) {
    cout << label << (firstMissingPositive(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 0}, 3);
    test("lc ex2", {3, 4, -1, 1}, 2);
    test("all too big", {7, 8, 9}, 1);
    test("duplicate", {1, 1}, 2);
}
