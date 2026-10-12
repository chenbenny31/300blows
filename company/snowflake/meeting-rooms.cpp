#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

using std::pair;
using std::vector;

// T: O(nlogn), S: O(n)
int minMeetingRooms(const vector<vector<int>>& meetings) {
    vector<pair<int, int>> events; // (time, +1/-1)

    for (const auto& m : meetings) {
        events.push_back({m[0], +1});
        events.push_back({m[1], -1});
    }

    std::sort(events.begin(), events.end());

    int rooms = 0;
    int minRooms = 0;
    for (const auto& e : events) {
        rooms += e.second;
        minRooms = std::max(minRooms, rooms); 
    }
    return minRooms;
}

#include <queue>
#include <functional>

using std::priority_queue;
using std::greater;

// T: O(nlogn), S: O(n)
int minMeetingRooms_2(vector<vector<int>>& meetings) {
    std::sort(meetings.begin(), meetings.end());
    priority_queue<int, vector<int>, greater<int>> ends;

    for (const auto& m : meetings) {
        int start = m[0];
        int end = m[1];

        if (!ends.empty() && ends.top() <= start) {
            ends.pop(); // re-use last room
        }
        ends.push(end);
    }

    return static_cast<int>(ends.size());
}


int main() {
    assert(minMeetingRooms({{15, 20}, {0, 30}, {5, 10}}) == 2);
    // Unsorted input and overlapping meetings.

    assert(minMeetingRooms({{1, 5}, {5, 10}}) == 1);
    // End before start at the same timestamp.

    assert(minMeetingRooms({{1, 10}, {2, 9}, {3, 8}}) == 3);
    // Nested intervals: all overlap.

    assert(minMeetingRooms({}) == 0);
}
