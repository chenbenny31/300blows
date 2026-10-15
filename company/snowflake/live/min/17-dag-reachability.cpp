// A15 · cf. LC 2192 (reversed direction): DAG, edges [u, v] = u -> v; for each node all nodes reachable via >= 1 edge, sorted, excluding itself; cycle -> {}.
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Kahn topo order, then union successors in reverse topo order, T: O(V*E + V^2), S: O(V^2)
vector<vector<int>> reachable(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> out(n);   // node -> direct successors
    vector<int> indeg(n, 0);      // node -> predecessors not yet placed
    for (const auto& e : edges) {
        out[e[0]].push_back(e[1]);
        indeg[e[1]]++;
    }
    std::queue<int> q;            // nodes with no unplaced predecessor
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }
    vector<int> topo;             // nodes in topological order
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);
        for (int v : out[u]) {
            if (--indeg[v] == 0) { q.push(v); }
        }
    }
    if ((int)topo.size() != n) { return {}; }   // cycle: not a DAG
    vector<vector<bool>> can(n, vector<bool>(n, false));   // can[u][w]: w reachable from u
    for (int i = n - 1; i >= 0; i--) {          // reverse topo: every successor's row is already complete
        int u = topo[i];
        for (int v : out[u]) {
            can[u][v] = true;
            for (int w = 0; w < n; w++) {
                if (can[v][w]) { can[u][w] = true; }
            }
        }
    }
    vector<vector<int>> result(n);
    for (int u = 0; u < n; u++) {
        for (int w = 0; w < n; w++) {
            if (can[u][w]) { result[u].push_back(w); }
        }
    }
    return result;
}

void test(const string& label, int n, const vector<vector<int>>& edges, const vector<vector<int>>& expect) {
    vector<vector<int>> got = reachable(n, edges);
    cout << (got == expect ? "PASS " : "FAIL ") << label << "\n";
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, {{1, 2, 3}, {3}, {3}, {}});          // common successor once
    test("single", 1, {}, {{}});                                                              // smallest input
    test("labels vs topo", 5, {{3, 1}, {1, 0}, {4, 2}}, {{}, {0}, {}, {0, 1}, {2}});          // index order != topo order
    test("duplicate edge", 2, {{0, 1}, {0, 1}}, {{1}, {}});                                   // no duplicate output
    test("cycle", 3, {{0, 1}, {1, 2}, {2, 1}}, {});                                           // contract: cycle -> empty
}
