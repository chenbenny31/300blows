// LC 974 Subarray Sums Divisible by K: count the non-empty contiguous subarrays of nums
// (values may be negative) whose sum is divisible by k.
// Trick: equal prefix remainders bound a divisible subarray; normalize negative %
// Recognize: "subarray sum divisible by / multiple of k" -> count prefix remainders.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// sum(j+1..i) % k == 0 iff prefix[i] and prefix[j] have the same remainder.
// T: O(n + k), S: O(k).
int subarraysDivByK(const vector<int>& nums, int k) {
    vector<int> count(k, 0);   // count[m] = earlier prefixes with remainder m
    count[0] = 1;              // empty prefix
    int sum = 0, result = 0;
    for (int x : nums) {
        sum += x;
        // C++ % can be negative: -2 % 5 == -2 must land in bucket 3
        int m = ((sum % k) + k) % k;
        result += count[m];
        count[m]++;
    }
    return result;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraysDivByK(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {4, 5, 0, -2, -3, 1}, 5, 7);
    test("lc ex2", {5}, 9, 0);
    test("negative", {-1, 2, 9}, 2, 2);
}
