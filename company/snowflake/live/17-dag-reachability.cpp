// A15 · cf. LC 2192 (reversed direction): DAG with nodes 0..n-1 and edges [u, v] (u -> v); for each node return every node reachable
// via one or more edges, excluding itself, sorted ascending.

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

class ReachabilityFinder {
public:
    // Kahn topo order, then union successors in reverse topo order, T: O(V*E + V^2), S: O(V^2)
    vector<vector<int>> reachable(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> out(n);        // node -> direct successors
        vector<int> indeg(n, 0);           // node -> predecessors not yet placed in topo order
        for (const auto& e : edges) {
            int u = e[0], v = e[1];
            out[u].push_back(v);
            indeg[v]++;
        }
        std::queue<int> q;                 // nodes with no unplaced predecessor
        for (int i = 0; i < n; i++) {
            if (indeg[i] == 0) { q.push(i); }
        }
        vector<int> topo;                  // nodes in topological order
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);
            for (int v : out[u]) {
                indeg[v]--;
                if (indeg[v] == 0) { q.push(v); }
            }
        }
        if ((int)topo.size() != n) { return {}; }   // cycle: not a DAG
        vector<vector<bool>> can(n, vector<bool>(n, false));   // can[u][w]: w reachable from u
        for (int i = n - 1; i >= 0; i--) {   // reverse topo: every successor's row is already complete
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

    // BFS from every node, T: O(V*(V+E)), S: O(V+E) beyond the output
    vector<vector<int>> reachable2(int n, const vector<vector<int>>& edges) {
        vector<vector<int>> out(n);        // node -> direct successors
        for (const auto& e : edges) {
            out[e[0]].push_back(e[1]);
        }
        vector<vector<int>> result(n);
        for (int s = 0; s < n; s++) {
            vector<bool> seen(n, false);   // node -> already reached from s
            std::queue<int> q;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : out[u]) {
                    if (seen[v]) { continue; }
                    seen[v] = true;
                    q.push(v);
                }
            }
            for (int w = 0; w < n; w++) {   // scan by index: sorted output without a sort
                if (seen[w] && w != s) { result[s].push_back(w); }
            }
        }
        return result;
    }
};

bool test(const string& label, int n, const vector<vector<int>>& edges, const vector<vector<int>>& expected) {
    ReachabilityFinder rf;
    vector<vector<int>> got = rf.reachable(n, edges);
    vector<vector<int>> got2 = rf.reachable2(n, edges);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << "\n";
    return false;
}

bool testCycle() {
    ReachabilityFinder rf;
    vector<vector<int>> got = rf.reachable(3, {{0, 1}, {1, 2}, {2, 1}});
    if (got.empty()) { return true; }
    cout << "FAIL cycle: expected empty result\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, {{1, 2, 3}, {3}, {3}, {}});        // common successor counted once
    ok &= test("single", 1, {}, {{}});                                                            // smallest input
    ok &= test("chain", 4, {{0, 1}, {1, 2}, {2, 3}}, {{1, 2, 3}, {2, 3}, {3}, {}});               // longest transitive path
    ok &= test("labels against topo", 5, {{3, 1}, {1, 0}, {4, 2}}, {{}, {0}, {}, {0, 1}, {2}});   // topo order != index order, two components
    ok &= test("duplicate edge", 2, {{0, 1}, {0, 1}}, {{1}, {}});                                 // repeated edge, no duplicate output
    ok &= test("no edges", 3, {}, {{}, {}, {}});                                                  // itself never included
    ok &= testCycle();                                                                            // contract: cycle -> empty
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
