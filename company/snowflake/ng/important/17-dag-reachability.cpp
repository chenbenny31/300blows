// A15 (cf. LC 2192, reversed): DAG with nodes 0..n-1, edge [u, v] means u -> v.
// For each node, the sorted list of nodes reachable via >= 1 edge (never itself).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::set;

// BFS from every node. T: O(V*(V+E)), S: O(V+E) beyond the output.
vector<vector<int>> reachable(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);   // graph[u] = direct successors of u (edge u -> v)
    for (const auto& e : edges) { graph[e[0]].push_back(e[1]); }
    vector<vector<int>> result(n);
    for (int s = 0; s < n; s++) {
        vector<bool> seen(n, false);   // seen[v] = v reached from s via >= 1 edge
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (seen[nxt]) { continue; }
                seen[nxt] = true;
                q.push(nxt);
            }
        }
        // scan by index: sorted output with no sort
        for (int v = 0; v < n; v++) {
            if (seen[v] && v != s) { result[s].push_back(v); }
        }
    }
    return result;
}

// follow-up: Kahn's order, then reverse topo: reach[u] = each successor v + reach[v].
// T: O(V+E) + O(E*V log V) for the unions, S: O(V^2). Cycle -> {}.
vector<vector<int>> reachableTopo(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);   // graph[u] = direct successors of u (edge u -> v)
    vector<int> indeg(n, 0);        // indeg[v] = predecessors of v not yet in order
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        indeg[e[1]]++;
    }
    queue<int> q;                   // nodes whose predecessors are all in order
    for (int u = 0; u < n; u++) {
        if (indeg[u] == 0) { q.push(u); }
    }
    vector<int> order;              // nodes in topological order
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            if (--indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    if ((int)order.size() != n) { return {}; }
    vector<set<int>> reach(n);      // reach[u] = nodes reachable from u, sorted, deduped
    // reverse topo order: every successor's set is already final
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        for (int v : graph[u]) {
            reach[u].insert(v);
            reach[u].insert(reach[v].begin(), reach[v].end());
        }
    }
    vector<vector<int>> result;
    for (int u = 0; u < n; u++) {
        result.push_back(vector<int>(reach[u].begin(), reach[u].end()));
    }
    return result;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<vector<int>>& want) {
    cout << label << (reachable(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, {{1, 2, 3}, {3}, {3}, {}});
    test("isolated", 3, {{0, 1}}, {{1}, {}, {}});
}
