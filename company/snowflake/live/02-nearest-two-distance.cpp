// A08 · LC 821 variant: array of 0/1/2, for each 1 (in order) return the index distance to its nearest 2, -1 if there is no 2.
// Follow-up B25 · LC 243 variant: smallest distance between any 1 and any 2, -1 if either is missing.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class NearestTwoFinder {
public:
    // two passes: nearest 2 on the left, then on the right, T: O(n), S: O(n)
    vector<int> nearestTwo(const vector<int>& nums) {
        int n = nums.size();
        vector<int> best(n, INT_MAX);   // index -> distance to the nearest 2 seen so far
        int last = -1;                  // index of the latest 2 to the left, -1 = none yet
        for (int i = 0; i < n; i++) {
            if (nums[i] == 2) { last = i; }
            if (nums[i] == 1 && last != -1) { best[i] = i - last; }
        }
        last = -1;                      // now: index of the nearest 2 to the right
        for (int i = n - 1; i >= 0; i--) {
            if (nums[i] == 2) { last = i; }
            if (nums[i] == 1 && last != -1) { best[i] = std::min(best[i], last - i); }
        }
        vector<int> result;             // one entry per 1, in input order
        for (int i = 0; i < n; i++) {
            if (nums[i] != 1) { continue; }
            result.push_back(best[i] == INT_MAX ? -1 : best[i]);
        }
        return result;
    }

    // collect positions of 2, binary search each 1, T: O(n log n), S: O(n)
    vector<int> nearestTwo2(const vector<int>& nums) {
        int n = nums.size();
        vector<int> twos;               // indices of 2s, ascending
        for (int i = 0; i < n; i++) {
            if (nums[i] == 2) { twos.push_back(i); }
        }
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] != 1) { continue; }
            if (twos.empty()) {
                result.push_back(-1);
                continue;
            }
            int pos = std::lower_bound(twos.begin(), twos.end(), i) - twos.begin();   // first 2 to the right
            int d = INT_MAX;
            if (pos < (int)twos.size()) { d = twos[pos] - i; }
            if (pos > 0) { d = std::min(d, i - twos[pos - 1]); }
            result.push_back(d);
        }
        return result;
    }

    // one pass remembering the latest 1 and latest 2, T: O(n), S: O(1)
    int closestPair(const vector<int>& nums) {
        int n = nums.size();
        int last1 = -1;                 // index of the latest 1, -1 = none yet
        int last2 = -1;                 // index of the latest 2, -1 = none yet
        int best = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (nums[i] == 1) {
                last1 = i;
                if (last2 != -1) { best = std::min(best, i - last2); }
            } else if (nums[i] == 2) {
                last2 = i;
                if (last1 != -1) { best = std::min(best, i - last1); }
            }
        }
        return best == INT_MAX ? -1 : best;
    }
};

bool test(const string& label, const vector<int>& nums, const vector<int>& expected) {
    NearestTwoFinder f;
    vector<int> got = f.nearestTwo(nums);
    vector<int> got2 = f.nearestTwo2(nums);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << "\n";
    return false;
}

bool testPair(const string& label, const vector<int>& nums, int expected) {
    NearestTwoFinder f;
    int got = f.closestPair(nums);
    if (got == expected) { return true; }
    cout << "FAIL pair " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("target", {1, 0, 2, 0, 1, 0, 0, 2, 1}, {2, 2, 1});   // target: left, either side, right
    ok &= test("right wins", {2, 0, 0, 1, 2}, {1});                 // right pass must improve the left value
    ok &= test("tie", {2, 0, 1, 0, 2}, {2});                        // equal distance both sides
    ok &= test("shared 2", {1, 1, 2}, {2, 1});                      // one 2 serves several 1s
    ok &= test("trailing 1s", {1, 0, 2, 0, 1, 1}, {2, 2, 3});       // only a left 2 for the tail
    ok &= test("no 2", {1, 0, 1}, {-1, -1});                        // missing 2 contract
    ok &= test("no 1", {2, 0, 2}, {});                              // nothing to report
    ok &= test("empty", {}, {});                                    // smallest input
    ok &= test("adjacent", {2, 1}, {1});                            // distance 1

    ok &= testPair("later improves", {1, 0, 0, 2, 0, 1}, 2);        // best pair is not the first one
    ok &= testPair("latest 1", {1, 1, 0, 2}, 2);                    // must use the most recent 1
    ok &= testPair("2 first", {2, 0, 1}, 2);                        // 2 before 1
    ok &= testPair("adjacent", {1, 2}, 1);                          // minimum possible
    ok &= testPair("no 2", {1, 0, 1}, -1);                          // missing 2
    ok &= testPair("no 1", {2, 0, 2}, -1);                          // missing 1
    ok &= testPair("empty", {}, -1);                                // smallest input
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
