// A13 · LC 210: prereq [a, b] means take b before a; return any valid order, {} on a cycle.
// Follow-up I04: only the courses needed for one target course, in a valid order.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS on indegrees. T: O(V+E), S: O(V+E).
vector<int> findOrder(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);   // graph[b] = courses unlocked by b (edge b -> a)
    vector<int> indeg(n, 0);        // indeg[a] = prereqs of a not yet taken
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }
    queue<int> q;                   // courses whose prereqs are all taken
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    // courses never reaching indegree 0 sit on or behind a cycle
    if ((int)order.size() != n) { return {}; }
    return order;
}

// 3-color DFS: false if a cycle is reachable from cur; postorder puts prereqs first.
bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state, vector<int>& order) {
    if (state[cur] == 2) { return true; }
    if (state[cur] == 1) { return false; }   // back edge: cur is on the current path
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (!dfs(nxt, graph, state, order)) { return false; }
    }
    state[cur] = 2;
    order.push_back(cur);
    return true;
}

// follow-up: DFS only from target, so unrelated courses and cycles are never touched.
// T: O(V+E), S: O(V+E).
vector<int> orderForTarget(int n, const vector<vector<int>>& prereqs, int target) {
    vector<vector<int>> graph(n);   // graph[a] = direct prereqs of a (edge a -> b)
    for (const auto& p : prereqs) { graph[p[0]].push_back(p[1]); }
    vector<int> state(n, 0);        // 0 = unvisited, 1 = on current path, 2 = done
    vector<int> order;
    if (!dfs(target, graph, state, order)) { return {}; }
    return order;
}

void test(const string& label, int n, const vector<vector<int>>& prereqs,
          const vector<int>& want) {
    cout << label << (findOrder(n, prereqs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unique order", 3, {{2, 0}, {2, 1}, {1, 0}}, {0, 1, 2});
    test("cycle", 3, {{1, 0}, {2, 1}, {1, 2}}, {});
}
