// A13 · LC 210: n courses 0..n-1, prereq [a, b] means b before a; return any valid order, {} on a cycle.
// Follow-up I04: only the courses needed for one target (its prerequisite closure + itself), in a valid order.

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

class CourseScheduler {
public:
    // Kahn's BFS on indegrees, T: O(V+E), S: O(V+E)
    vector<int> findOrder(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> next(n);       // course -> courses that require it
        vector<int> indeg(n, 0);           // course -> prereqs not yet taken
        for (const auto& p : prereqs) {
            int a = p[0], b = p[1];
            next[b].push_back(a);
            indeg[a]++;
        }
        std::queue<int> q;                 // courses with all prereqs taken
        for (int c = 0; c < n; c++) {
            if (indeg[c] == 0) { q.push(c); }
        }
        vector<int> order;
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            order.push_back(cur);
            for (int nxt : next[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) { q.push(nxt); }
            }
        }
        if ((int)order.size() != n) { return {}; }   // leftover courses sit on or behind a cycle
        return order;
    }

    // DFS three colors, postorder over prereq edges puts prereqs first, T: O(V+E), S: O(V+E)
    vector<int> findOrder2(int n, const vector<vector<int>>& prereqs) {
        vector<vector<int>> need(n);       // course -> its direct prereqs
        for (const auto& p : prereqs) { need[p[0]].push_back(p[1]); }
        vector<int> state(n, 0);           // 0 = unvisited, 1 = on the DFS path, 2 = done
        vector<int> order;
        for (int c = 0; c < n; c++) {
            if (!dfs(c, need, state, order)) { return {}; }
        }
        return order;
    }

    // I04: same DFS started only from target, so unrelated courses and unrelated cycles are ignored
    // T: O(V'+E'), S: O(V+E), V'/E' = courses/edges in target's closure
    vector<int> orderForTarget(int n, const vector<vector<int>>& prereqs, int target) {
        vector<vector<int>> need(n);       // course -> its direct prereqs
        for (const auto& p : prereqs) { need[p[0]].push_back(p[1]); }
        vector<int> state(n, 0);           // 0 = unvisited, 1 = on the DFS path, 2 = done
        vector<int> order;
        if (!dfs(target, need, state, order)) { return {}; }
        return order;
    }

private:
    // false if a cycle is reachable from c; appends c after all its prereqs
    bool dfs(int c, const vector<vector<int>>& need, vector<int>& state, vector<int>& order) {
        if (state[c] == 2) { return true; }
        if (state[c] == 1) { return false; }   // back edge: c is already on the current path
        state[c] = 1;
        for (int p : need[c]) {
            if (!dfs(p, need, state, order)) { return false; }
        }
        state[c] = 2;
        order.push_back(c);
        return true;
    }
};

// order holds exactly the courses in `expectedSet` once each, and every prereq inside the set comes first
bool isValid(const vector<vector<int>>& prereqs, const vector<int>& order, const vector<int>& expectedSet) {
    vector<int> sortedOrder = order;
    vector<int> sortedSet = expectedSet;
    std::sort(sortedOrder.begin(), sortedOrder.end());
    std::sort(sortedSet.begin(), sortedSet.end());
    if (sortedOrder != sortedSet) { return false; }
    std::unordered_map<int, int> pos;  // course -> index in order
    int k = order.size();
    for (int i = 0; i < k; i++) { pos[order[i]] = i; }
    for (const auto& p : prereqs) {
        int a = p[0], b = p[1];
        if (pos.count(a) == 0) { continue; }
        if (pos.count(b) == 0 || pos[b] > pos[a]) { return false; }   // a taken -> b must be taken earlier
    }
    return true;
}

vector<int> allCourses(int n) {
    vector<int> all;
    for (int c = 0; c < n; c++) { all.push_back(c); }
    return all;
}

bool test(const string& label, int n, const vector<vector<int>>& prereqs, bool possible) {
    CourseScheduler cs;
    vector<int> got = cs.findOrder(n, prereqs);
    vector<int> got2 = cs.findOrder2(n, prereqs);
    bool pass = possible ? isValid(prereqs, got, allCourses(n)) && isValid(prereqs, got2, allCourses(n))
                         : got.empty() && got2.empty();
    if (!pass) { cout << "FAIL " << label << "\n"; }
    return pass;
}

// expectedSet empty means a cycle blocks the target
bool testTarget(const string& label, int n, const vector<vector<int>>& prereqs, int target,
                const vector<int>& expectedSet) {
    CourseScheduler cs;
    vector<int> got = cs.orderForTarget(n, prereqs, target);
    bool pass = expectedSet.empty() ? got.empty() : isValid(prereqs, got, expectedSet);
    if (!pass) { cout << "FAIL target " << label << "\n"; }
    return pass;
}

int main() {
    bool ok = true;
    ok &= test("lc ex1", 2, {{1, 0}}, true);                                   // LC example 1
    ok &= test("lc ex2", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);           // LC example 2, diamond
    ok &= test("single", 1, {}, true);                                         // smallest input
    ok &= test("two-cycle", 2, {{1, 0}, {0, 1}}, false);                       // cycle
    ok &= test("self loop", 2, {{0, 0}}, false);                               // self prerequisite
    ok &= test("partial cycle", 4, {{1, 0}, {2, 1}, {1, 2}}, false);           // free courses must not hide the cycle
    ok &= test("duplicate edge", 3, {{1, 0}, {1, 0}, {2, 1}}, true);           // indegree counted twice, decremented twice

    vector<vector<int>> mixed = {{1, 0}, {2, 1}, {4, 3}, {5, 6}, {6, 5}};      // 0 <- 1 <- 2, 3 <- 4, cycle 5 <-> 6
    ok &= testTarget("chain", 7, mixed, 2, {0, 1, 2});                         // only the closure, not 3..6
    ok &= testTarget("no prereq", 7, mixed, 3, {3});                           // target alone
    ok &= testTarget("cycle elsewhere", 7, mixed, 4, {3, 4});                  // unrelated cycle is ignored
    ok &= testTarget("cycle on path", 7, mixed, 5, {});                        // target on a cycle
    ok &= testTarget("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, 3, {0, 1, 2, 3});   // shared prereq output once
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
