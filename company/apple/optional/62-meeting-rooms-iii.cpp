// LC 2402: rooms 0..n-1, meetings [start, end) with unique starts. Each meeting takes the
// lowest free room; if none is free it waits for the earliest-free room and keeps its
// duration. Return the room that held the most meetings (ties -> lowest number).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::sort;
using std::max_element;

// Sort by start; heap of free room ids + heap of (end time, room) for busy rooms.
// T: O(m log m + m log n), S: O(n).
int mostBooked(int n, vector<vector<int>> meetings) {
    sort(meetings.begin(), meetings.end());
    priority_queue<int, vector<int>, greater<int>> freeRooms;   // free ids, lowest first
    for (int r = 0; r < n; r++) { freeRooms.push(r); }
    // (end time, room) of busy rooms; long long: delays can push ends past int
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> busy;
    vector<int> count(n, 0);   // count[r] = meetings held in room r
    for (const auto& m : meetings) {
        long long begin = m[0], duration = m[1] - m[0];
        // release every room that is done by this start
        while (!busy.empty() && busy.top().first <= begin) {
            freeRooms.push(busy.top().second);
            busy.pop();
        }
        int room;
        if (freeRooms.empty()) {
            // delayed: start when the earliest room frees (ties -> lower room)
            begin = busy.top().first;
            room = busy.top().second;
            busy.pop();
        } else {
            room = freeRooms.top();
            freeRooms.pop();
        }
        busy.push({begin + duration, room});
        count[room]++;
    }
    // max_element returns the first maximum = lowest room number
    return max_element(count.begin(), count.end()) - count.begin();
}

void test(const string& label, int n, const vector<vector<int>>& meetings, int want) {
    cout << label << (mostBooked(n, meetings) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", 2, {{0, 10}, {1, 5}, {2, 7}, {3, 4}}, 0);
    test("lc ex2", 3, {{1, 20}, {2, 10}, {3, 5}, {4, 9}, {6, 8}}, 1);
    test("unsorted", 1, {{5, 6}, {0, 1}}, 0);
}
