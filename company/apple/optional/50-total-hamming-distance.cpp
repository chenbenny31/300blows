// LC 477: sum of Hamming distances over all pairs of nums (0 <= nums[i] < 2^31).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Per bit, every (one, zero) pair differs there: add ones * zeros. T: O(32n), S: O(1).
long long totalHammingDistance(const vector<int>& nums) {
    int n = nums.size();
    long long total = 0;
    for (int b = 0; b < 31; b++) {
        int ones = 0;   // numbers with bit b set
        for (int x : nums) { ones += (x >> b) & 1; }
        total += 1LL * ones * (n - ones);
    }
    return total;
}

void test(const string& label, const vector<int>& nums, long long want) {
    cout << label << (totalHammingDistance(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {4, 14, 2}, 6);
    test("all equal", {7, 7}, 0);
    test("one", {5}, 0);
}
