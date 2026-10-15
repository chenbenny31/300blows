// B08 · LC 56: merge overlapping closed intervals [start, end]; touching ones merge. Follow-up: half-open [start, end), touching stays apart.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::string;
using std::vector;

// sort by start, extend the last merged interval, T: O(nlogn), S: O(n)
vector<vector<int>> merge(vector<vector<int>> intervals) {
    std::sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;   // disjoint intervals in start order
    for (const auto& cur : intervals) {
        if (!merged.empty() && cur[0] <= merged.back()[1]) {         // closed: start == last end still overlaps
            merged.back()[1] = std::max(merged.back()[1], cur[1]);   // max: cur may sit inside the last one
        } else {
            merged.push_back(cur);
        }
    }
    return merged;
}

// follow-up: same sweep, only strict overlap merges, T: O(nlogn), S: O(n)
vector<vector<int>> mergeHalfOpen(vector<vector<int>> intervals) {
    std::sort(intervals.begin(), intervals.end());
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

void test(const string& label, const vector<vector<int>>& intervals, const vector<vector<int>>& expect) {
    vector<vector<int>> got = merge(intervals);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

void testHalfOpen(const string& label, const vector<vector<int>>& intervals, const vector<vector<int>>& expect) {
    vector<vector<int>> got = mergeHalfOpen(intervals);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    test("lc1", {{1, 3}, {2, 6}, {8, 10}, {15, 18}}, {{1, 6}, {8, 10}, {15, 18}});   // LC example 1
    test("touching", {{1, 4}, {4, 5}}, {{1, 5}});                                    // LC example 2
    test("empty", {}, {});                                                           // smallest input
    test("contained", {{1, 10}, {2, 3}, {4, 5}}, {{1, 10}});                         // needs max, not cur end
    test("unsorted extremes", {{0, INT_MAX}, {INT_MIN, 0}}, {{INT_MIN, INT_MAX}});   // must sort, int bounds

    testHalfOpen("touch", {{1, 2}, {2, 3}}, {{1, 2}, {2, 3}});                       // touching stays apart
    testHalfOpen("overlap", {{2, 4}, {1, 3}, {6, 7}}, {{1, 4}, {6, 7}});             // real overlap merges
    testHalfOpen("contained", {{1, 10}, {2, 3}}, {{1, 10}});                         // containment
}
