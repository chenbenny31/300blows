// LC 169 Majority Element: return the element appearing more than n/2 times
// (guaranteed to exist).
// Follow-up LC 229: return all elements appearing more than n/3 times (none guaranteed).
// Trick: Boyer-Moore voting: a different element cancels one vote of the candidate
// Recognize: "more than n/k times" in O(1) space -> keep k-1 candidates.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Boyer-Moore voting: each cancel removes one majority and one other element, so the
// majority (> n/2) cannot be fully cancelled. T: O(n), S: O(1).
int majorityElement(const vector<int>& nums) {
    int candidate = 0, count = 0;   // count = unmatched votes for candidate
    for (int x : nums) {
        if (count == 0) { candidate = x; }
        count += (x == candidate) ? 1 : -1;
    }
    return candidate;
}

// follow-up: two candidates, each cancel drops 3 distinct values; then verify counts.
// T: O(n), S: O(1).
vector<int> majorityElementThird(const vector<int>& nums) {
    int c1 = 0, c2 = 1, cnt1 = 0, cnt2 = 0;   // c1 != c2 so they never share votes
    for (int x : nums) {
        // match a candidate before reusing an empty slot
        if (x == c1) {
            cnt1++;
        } else if (x == c2) {
            cnt2++;
        } else if (cnt1 == 0) {
            c1 = x;
            cnt1 = 1;
        } else if (cnt2 == 0) {
            c2 = x;
            cnt2 = 1;
        } else {
            cnt1--;
            cnt2--;
        }
    }
    // survivors are only candidates: no majority is guaranteed, so count again
    cnt1 = 0;
    cnt2 = 0;
    for (int x : nums) {
        if (x == c1) { cnt1++; }
        if (x == c2) { cnt2++; }
    }
    int n = nums.size();
    vector<int> result;
    if (cnt1 > n / 3) { result.push_back(c1); }
    if (cnt2 > n / 3) { result.push_back(c2); }
    return result;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (majorityElement(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {3, 2, 3}, 3);
    test("lc ex2", {2, 2, 1, 1, 1, 2, 2}, 2);
}
