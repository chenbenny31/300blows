// A05 · LC 1136: courses 1..n, relation [prev, next]; any number of unlocked courses
// per semester. Return the min number of semesters, -1 on a cycle.
// Follow-up LC 2050: course i takes time[i-1]; min total time with unlimited parallelism.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::max;

// Kahn's BFS layer by layer; one layer = one semester. T: O(V+E), S: O(V+E).
int minSemesters(int n, const vector<vector<int>>& relations) {
    vector<vector<int>> graph(n + 1);   // graph[prev] = courses unlocked by prev
    vector<int> indeg(n + 1, 0);        // indeg[c] = prereqs of c not yet taken
    for (const auto& r : relations) {
        graph[r[0]].push_back(r[1]);
        indeg[r[1]]++;
    }
    queue<int> q;                       // courses takeable in the current semester
    for (int c = 1; c <= n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    int semesters = 0, taken = 0;
    while (!q.empty()) {
        semesters++;
        // freeze the layer size: courses pushed now belong to the next semester
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            taken++;
            for (int nxt : graph[cur]) {
                if (--indeg[nxt] == 0) { q.push(nxt); }
            }
        }
    }
    // untaken courses sit on or behind a cycle
    return taken == n ? semesters : -1;
}

// follow-up: Kahn's order; a course starts when its slowest prereq finishes.
// T: O(V+E), S: O(V+E).
long long minTime(int n, const vector<vector<int>>& relations, const vector<int>& time) {
    vector<vector<int>> graph(n + 1);   // graph[prev] = courses unlocked by prev
    vector<int> indeg(n + 1, 0);        // indeg[c] = prereqs of c not yet finished
    for (const auto& r : relations) {
        graph[r[0]].push_back(r[1]);
        indeg[r[1]]++;
    }
    vector<long long> start(n + 1, 0);  // start[c] = latest finish of c's prereqs so far
    queue<int> q;                       // courses whose prereqs are all finished
    for (int c = 1; c <= n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    long long total = 0;
    int done = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        done++;
        // start[cur] is final: every prereq was popped before cur
        long long finish = start[cur] + time[cur - 1];
        total = max(total, finish);
        for (int nxt : graph[cur]) {
            start[nxt] = max(start[nxt], finish);
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return done == n ? total : -1;
}

void test(const string& label, int n, const vector<vector<int>>& relations, int want) {
    cout << label << (minSemesters(n, relations) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two prereqs", 3, {{1, 3}, {2, 3}}, 2);
    test("shortcut edge", 3, {{1, 2}, {2, 3}, {1, 3}}, 3);
    test("partial cycle", 4, {{1, 2}, {2, 3}, {3, 2}}, -1);
}
