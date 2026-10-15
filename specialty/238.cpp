// LC 238 Product of Array Except Self: answer[i] = product of all nums except nums[i],
// without division, in O(n); the output array does not count as extra space.
// Trick: answer[i] = (product left of i) * (product right of i); two passes
// Recognize: "all except self" without division -> prefix * suffix.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Left products into answer, then multiply in a running right product.
// No division, so zeros need no special case. T: O(n), S: O(1) extra.
vector<int> productExceptSelf(const vector<int>& nums) {
    int n = nums.size();
    vector<int> answer(n, 1);   // after pass 1: answer[i] = product of nums[0..i-1]
    for (int i = 1; i < n; i++) { answer[i] = answer[i - 1] * nums[i - 1]; }
    int right = 1;              // product of nums[i+1..n-1]
    for (int i = n - 1; i >= 0; i--) {
        answer[i] *= right;
        right *= nums[i];
    }
    return answer;
}

void test(const string& label, const vector<int>& nums, const vector<int>& want) {
    cout << label << (productExceptSelf(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 2, 3, 4}, {24, 12, 8, 6});
    test("zero", {-1, 1, 0, -3, 3}, {0, 0, 9, 0, 0});
}
