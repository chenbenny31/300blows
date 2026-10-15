// I01 · LC 253: minimum rooms for half-open [start, end) meetings (end == start reuses).
// Follow-ups: sweep line for the same answer; LC 252 can one person attend all?
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::sort;
using std::pair;
using std::max;

// Sort by start; min-heap of each room's current end time. T: O(n log n), S: O(n).
int minRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // ends = end time of the meeting in each room, earliest on top
    priority_queue<int, vector<int>, greater<int>> ends;
    for (const auto& m : intervals) {
        // <=: a room freed exactly at start is reusable
        if (!ends.empty() && ends.top() <= m[0]) { ends.pop(); }
        ends.push(m[1]);
    }
    return ends.size();   // grows only when a room is added, so size = peak
}

// follow-up: sweep line. +1 at each start, -1 at each end; the peak running sum
// is the answer. T: O(n log n), S: O(n).
int minRoomsSweep(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;   // (time, +1 start / -1 end)
    for (const auto& m : intervals) {
        events.push_back({m[0], 1});
        events.push_back({m[1], -1});
    }
    // pair order puts (t, -1) before (t, +1): a room frees before the next meeting takes it
    sort(events.begin(), events.end());
    int cur = 0, best = 0;
    for (const auto& [time, delta] : events) {
        cur += delta;
        best = max(best, cur);
    }
    return best;
}

// follow-up: sort, then a meeting starting before the previous one ends is a clash.
// T: O(n log n).
bool canAttendAll(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    int n = intervals.size();
    for (int i = 1; i < n; i++) {
        if (intervals[i][0] < intervals[i - 1][1]) { return false; }
    }
    return true;
}

// follow-up (bus, talk only): trips {start, end, from, to}; a bus is reused only where it
// parked, so keep one min-heap of free times per station and add a bus when none is free.

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minRooms(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {{0, 30}, {5, 10}, {15, 20}}, 2);
    test("back to back", {{5, 10}, {1, 5}}, 1);
    test("empty", {}, 0);
}
