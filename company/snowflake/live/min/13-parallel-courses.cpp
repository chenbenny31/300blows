// A05 · LC 1136: courses 1..n, [prev, next]; any number of unlocked courses per semester; min semesters, -1 on a cycle. Follow-up LC 2050: per-course time, min total time.
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using std::cout;
using std::vector;

// Kahn's BFS by layer, one layer = one semester, T: O(V+E), S: O(V+E)
int minSemesters(int n, const vector<vector<int>>& relations) {
    vector<vector<int>> next(n + 1);   // course -> courses it unlocks
    vector<int> indeg(n + 1, 0);       // course -> prereqs not yet taken
    for (const auto& r : relations) {
        next[r[0]].push_back(r[1]);
        indeg[r[1]]++;
    }
    std::queue<int> q;                 // courses takeable this semester
    for (int c = 1; c <= n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    int semesters = 0, taken = 0;
    while (!q.empty()) {
        semesters++;
        int sz = q.size();             // freeze layer size before pushing the next layer
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            taken++;
            for (int nxt : next[cur]) {
                if (--indeg[nxt] == 0) { q.push(nxt); }
            }
        }
    }
    return taken == n ? semesters : -1;   // untaken courses sit on or behind a cycle
}

// follow-up: Kahn's order, finish[v] = time[v] + max finish over prereqs (longest path), T: O(V+E), S: O(V+E)
long long minTime(int n, const vector<vector<int>>& relations, const vector<int>& time) {
    vector<vector<int>> next(n + 1);   // course -> courses it unlocks
    vector<int> indeg(n + 1, 0);       // course -> prereqs not yet finished
    for (const auto& r : relations) {
        next[r[0]].push_back(r[1]);
        indeg[r[1]]++;
    }
    vector<long long> start(n + 1, 0); // course -> latest finish among its prereqs so far
    std::queue<int> q;                 // courses whose prereqs are all finished
    for (int c = 1; c <= n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    long long total = 0;
    int done = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        done++;
        long long finish = start[cur] + time[cur - 1];   // start[cur] is final once indeg hit 0
        total = std::max(total, finish);
        for (int nxt : next[cur]) {
            start[nxt] = std::max(start[nxt], finish);   // wait for the slowest prereq
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return done == n ? total : -1;
}

void test(int n, const vector<vector<int>>& relations, int expect) {
    int got = minSemesters(n, relations);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testTime(int n, const vector<vector<int>>& relations, const vector<int>& time, long long expect) {
    long long got = minTime(n, relations, time);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test(3, {{1, 3}, {2, 3}}, 2);                                   // LC 1136 example 1
    test(1, {}, 1);                                                 // smallest input
    test(4, {{1, 2}, {2, 4}, {1, 4}, {3, 4}}, 3);                   // uneven diamond: first-reach depth says 2
    test(4, {{1, 2}, {2, 3}, {3, 2}}, -1);                          // partial cycle

    testTime(5, {{1, 5}, {2, 5}, {3, 5}, {3, 4}, {4, 5}}, {1, 2, 3, 4, 5}, 12);           // LC 2050 example 2
    testTime(1, {}, {4}, 4);                                                               // smallest input
    testTime(3, {{2, 3}}, {10, 1, 5}, 10);                                                 // summing layer maxima gives 15
    testTime(3, {{1, 2}, {2, 3}}, {1000000000, 1000000000, 1000000000}, 3000000000LL);    // passes INT_MAX
    testTime(2, {{1, 2}, {2, 1}}, {1, 1}, -1);                                             // cycle
}
