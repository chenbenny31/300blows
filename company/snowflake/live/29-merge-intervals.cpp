// B08 / LC 56: merge all overlapping closed intervals [start, end]; touching ones like [1,2],[2,3] merge.

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

class IntervalMerger {
public:
    // sort by start, extend the last merged interval, T: O(n log n), S: O(n) for the copy and output
    vector<vector<int>> merge(vector<vector<int>> intervals) {
        std::sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) { return a[0] < b[0]; });
        vector<vector<int>> merged;   // disjoint intervals in start order
        for (const auto& cur : intervals) {
            // closed intervals: start == last end still overlaps
            if (!merged.empty() && cur[0] <= merged.back()[1]) {
                merged.back()[1] = std::max(merged.back()[1], cur[1]);   // max: cur may sit inside the last one
            } else {
                merged.push_back(cur);
            }
        }
        return merged;
    }

    // half-open [start, end): same sweep, only strict overlap merges, T: O(n log n), S: O(n)
    vector<vector<int>> mergeHalfOpen(vector<vector<int>> intervals) {
        std::sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) { return a[0] < b[0]; });
        vector<vector<int>> merged;   // disjoint intervals in start order
        for (const auto& cur : intervals) {
            if (!merged.empty() && cur[0] < merged.back()[1]) {
                merged.back()[1] = std::max(merged.back()[1], cur[1]);
            } else {
                merged.push_back(cur);
            }
        }
        return merged;
    }
};

bool test(const string& label, const vector<vector<int>>& intervals, const vector<vector<int>>& expected) {
    IntervalMerger im;
    vector<vector<int>> got = im.merge(intervals);
    if (got == expected) { return true; }
    cout << "FAIL " << label << "\n";
    return false;
}

bool testHalfOpen(const string& label, const vector<vector<int>>& intervals, const vector<vector<int>>& expected) {
    IntervalMerger im;
    vector<vector<int>> got = im.mergeHalfOpen(intervals);
    if (got == expected) { return true; }
    cout << "FAIL half-open " << label << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("lc1", {{1, 3}, {2, 6}, {8, 10}, {15, 18}}, {{1, 6}, {8, 10}, {15, 18}});   // LC example 1
    ok &= test("lc2", {{1, 4}, {4, 5}}, {{1, 5}});                                         // LC example 2: touching merges
    ok &= test("empty", {}, {});                                                           // smallest input
    ok &= test("single", {{5, 5}}, {{5, 5}});                                              // one point interval
    ok &= test("unsorted", {{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}});                  // input not sorted
    ok &= test("contained", {{1, 10}, {2, 3}, {4, 5}}, {{1, 10}});                         // needs max, not cur end
    ok &= test("chain", {{1, 2}, {2, 3}, {3, 4}}, {{1, 4}});                               // closed chain of touches
    ok &= test("extremes", {{0, INT_MAX}, {INT_MIN, 0}}, {{INT_MIN, INT_MAX}});            // int boundary values
    ok &= testHalfOpen("touch", {{1, 2}, {2, 3}}, {{1, 2}, {2, 3}});                       // half-open: touching stays apart
    ok &= testHalfOpen("overlap", {{2, 4}, {1, 3}, {6, 7}}, {{1, 4}, {6, 7}});             // half-open: real overlap merges
    ok &= testHalfOpen("contained", {{1, 10}, {2, 3}}, {{1, 10}});                         // half-open: containment
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
