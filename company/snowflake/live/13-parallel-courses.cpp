// A05 · LC 1136: courses 1..n, relation [prev, next]; take any number of unlocked courses per semester, return min semesters, -1 on a cycle.
// Follow-up LC 2050: course i takes time[i-1] months and starts as soon as its last prereq ends; return min total time, -1 on a cycle.

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

class CoursePlanner {
public:
    // Kahn's BFS by layer, one layer = one semester, T: O(V+E), S: O(V+E)
    int minSemesters(int n, const vector<vector<int>>& relations) {
        vector<vector<int>> next(n + 1);   // course -> courses it unlocks
        vector<int> indeg(n + 1, 0);       // course -> prereqs not yet taken
        for (const auto& r : relations) {
            next[r[0]].push_back(r[1]);
            indeg[r[1]]++;
        }
        std::queue<int> q;                 // courses takeable in the current semester
        for (int c = 1; c <= n; c++) {
            if (indeg[c] == 0) { q.push(c); }
        }
        int semesters = 0;
        int taken = 0;
        while (!q.empty()) {
            semesters++;
            int sz = q.size();             // freeze the layer size before pushing the next layer
            for (int i = 0; i < sz; i++) {
                int cur = q.front();
                q.pop();
                taken++;
                for (int nxt : next[cur]) {
                    indeg[nxt]--;
                    if (indeg[nxt] == 0) { q.push(nxt); }
                }
            }
        }
        return taken == n ? semesters : -1;   // untaken courses sit on or behind a cycle
    }

    // Kahn's order, finish[v] = time[v] + max finish over v's prereqs, T: O(V+E), S: O(V+E)
    long long minTime(int n, const vector<vector<int>>& relations, const vector<int>& time) {
        vector<vector<int>> next(n + 1);   // course -> courses it unlocks
        vector<int> indeg(n + 1, 0);       // course -> prereqs not yet finished
        for (const auto& r : relations) {
            next[r[0]].push_back(r[1]);
            indeg[r[1]]++;
        }
        vector<long long> start(n + 1, 0); // course -> latest finish among its prereqs processed so far
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
                indeg[nxt]--;
                if (indeg[nxt] == 0) { q.push(nxt); }
            }
        }
        return done == n ? total : -1;
    }
};

bool testSemesters(const string& label, int n, const vector<vector<int>>& relations, int expected) {
    CoursePlanner cp;
    int got = cp.minSemesters(n, relations);
    if (got == expected) { return true; }
    cout << "FAIL semesters " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool testTime(const string& label, int n, const vector<vector<int>>& relations,
              const vector<int>& time, long long expected) {
    CoursePlanner cp;
    long long got = cp.minTime(n, relations, time);
    if (got == expected) { return true; }
    cout << "FAIL time " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= testSemesters("lc ex1", 3, {{1, 3}, {2, 3}}, 2);                              // LC 1136 example 1
    ok &= testSemesters("lc ex2", 3, {{1, 2}, {2, 3}, {3, 1}}, -1);                     // LC 1136 example 2, full cycle
    ok &= testSemesters("single", 1, {}, 1);                                            // smallest input
    ok &= testSemesters("chain+isolated", 4, {{1, 2}, {2, 3}}, 3);                      // longest chain decides
    ok &= testSemesters("uneven diamond", 4, {{1, 2}, {2, 4}, {1, 4}, {3, 4}}, 3);      // first-reach depth would say 2
    ok &= testSemesters("partial cycle", 4, {{1, 2}, {2, 3}, {3, 2}}, -1);              // cycle not covering every course
    ok &= testSemesters("self loop", 2, {{1, 1}}, -1);                                  // self-dependency

    ok &= testTime("lc ex1", 3, {{1, 3}, {2, 3}}, {3, 2, 5}, 8);                                     // LC 2050 example 1
    ok &= testTime("lc ex2", 5, {{1, 5}, {2, 5}, {3, 5}, {3, 4}, {4, 5}}, {1, 2, 3, 4, 5}, 12);      // LC 2050 example 2
    ok &= testTime("single", 1, {}, {4}, 4);                                                         // smallest input
    ok &= testTime("layer max trap", 3, {{2, 3}}, {10, 1, 5}, 10);                                   // summing layer maxima gives 15
    ok &= testTime("overflow", 3, {{1, 2}, {2, 3}}, {1000000000, 1000000000, 1000000000}, 3000000000LL); // passes INT_MAX
    ok &= testTime("cycle", 2, {{1, 2}, {2, 1}}, {1, 1}, -1);                                        // cycle contract
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
