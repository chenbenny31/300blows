// LC 128: length of the longest run of consecutive integers in unsorted nums, in O(n).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::max;

// Hash set; count up only from x with no x - 1 (the head of a run). T: O(n), S: O(n).
int longestConsecutive(const vector<int>& nums) {
    unordered_set<int> present(nums.begin(), nums.end());   // distinct values of nums
    int best = 0;
    for (int x : present) {
        // non-heads skip, so each run is walked once in total
        if (present.count(x - 1)) { continue; }
        int len = 1;
        while (present.count(x + len)) { len++; }
        best = max(best, len);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (longestConsecutive(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {100, 4, 200, 1, 3, 2}, 4);
    test("duplicates", {1, 2, 0, 1}, 3);
    test("empty", {}, 0);
}
