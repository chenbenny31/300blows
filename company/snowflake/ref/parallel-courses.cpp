// n courses 1..n, relation [prev, next]: prev must finish before next; any number of courses at once
// LC1136: each course takes one semester, return min semesters, -1 on a cycle (J10557 NG)
// LC2050 follow-up: course i takes time[i] months, return min months to finish all

// 1-indexed courses?
// unlimited courses per semester, or at most k?
// cycle: return -1?
// follow-up: next course starts right when its last prereq finishes?

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class ParallelCourses {
public:
    // Kahn by level, one level = one semester, T: O(V+E), S: O(V+E)
    int minSemesters(int n, const vector<vector<int>>& relations) {
        vector<vector<int>> graph(n); // course -> courses it unlocks
        vector<int> indeg(n, 0); // course -> prereqs not yet taken
        for (const auto& r : relations) {
            graph[r[0] - 1].push_back(r[1] - 1); // input is 1-indexed
            indeg[r[1] - 1]++;
        }

        vector<int> frontier; // courses takeable this semester
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                frontier.push_back(i);
            }
        }
        int semesters = 0;
        int taken = 0;
        while (!frontier.empty()) {
            semesters++;
            taken += static_cast<int>(frontier.size());
            vector<int> nextFrontier;
            for (int cur : frontier) {
                for (int nxt : graph[cur]) {
                    indeg[nxt]--;
                    if (indeg[nxt] == 0) {
                        nextFrontier.push_back(nxt);
                    }
                }
            }
            frontier = nextFrontier;
        }
        return taken == n ? semesters : -1; // leftovers sit on or behind a cycle
    }

    // follow-up: finish[v] = time[v] + max finish over v's prereqs
    // Kahn, a course's start is final once it is popped, T: O(V+E), S: O(V+E)
    int minMonths(int n, const vector<vector<int>>& relations, const vector<int>& time) {
        vector<vector<int>> graph(n); // course -> courses it unlocks
        vector<int> indeg(n, 0); // course -> prereqs not yet finished
        for (const auto& r : relations) {
            graph[r[0] - 1].push_back(r[1] - 1);
            indeg[r[1] - 1]++;
        }

        vector<int> start(n, 0); // course -> earliest start = latest finish among prereqs seen so far
        vector<int> order;
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) {
                order.push_back(i);
            }
        }
        int total = 0;
        for (int i = 0; i < static_cast<int>(order.size()); i++) {
            int u = order[i];
            int finish = start[u] + time[u];
            total = std::max(total, finish);
            for (int v : graph[u]) {
                start[v] = std::max(start[v], finish); // v waits for its slowest prereq
                indeg[v]--;
                if (indeg[v] == 0) {
                    order.push_back(v);
                }
            }
        }
        return static_cast<int>(order.size()) == n ? total : -1;
    }
};

int main() {
    ParallelCourses pc;

    assert(pc.minSemesters(3, {{1, 3}, {2, 3}}) == 2); // target: two prereqs taken together
    assert(pc.minSemesters(4, {{1, 2}, {2, 3}}) == 3); // edge: longest chain decides, course 4 rides along
    assert(pc.minSemesters(3, {{1, 2}, {2, 3}, {3, 1}}) == -1); // fault: cycle
    assert(pc.minSemesters(1, {}) == 1); // null: one course, no relations

    assert(pc.minMonths(3, {{1, 3}, {2, 3}}, {3, 2, 5}) == 8); // target: waits for the slower prereq
    assert(pc.minMonths(2, {}, {1, 10}) == 10); // edge: no prereqs, longest course decides
    assert(pc.minMonths(2, {{1, 2}, {2, 1}}, {1, 1}) == -1); // fault: cycle
    assert(pc.minMonths(1, {}, {4}) == 4); // null: one course

    std::cout << "all tests passed\n";
    return 0;
}
